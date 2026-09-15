#include "xbox360/netplay_protocol.h"
#include "xbox360/netplay_diagnostic.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <vector>
#include <algorithm>

using namespace mknet;

static unsigned checks = 0;
#define CHECK(x) do { ++checks; if(!(x)){ fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #x); exit(1); } } while(0)

static Pad make_input(unsigned frame, unsigned slot) {
    Pad p = { uint16_t((frame * 19u + slot * 101u) & 0xffffu),
              int8_t((frame * 3u + slot * 17u) & 0xffu),
              int8_t((frame * 7u - slot * 13u) & 0xffu) };
    return p;
}

static uint32_t hash_step(uint32_t h, Pad p) {
    h = (h ^ p.buttons) * 16777619u;
    h = (h ^ uint8_t(p.x)) * 16777619u;
    h = (h ^ uint8_t(p.y)) * 16777619u;
    return h;
}

struct Message {
    unsigned due;
    unsigned seq;
    unsigned from;
    unsigned to;
    int n;
    uint8_t data[MAX_PACKET];
};
struct LaterFirst {
    bool operator()(const Message &a, const Message &b) const {
        if (a.due != b.due) return a.due > b.due;
        return a.seq > b.seq;
    }
};

static uint32_t rng_state = 1;
static unsigned seq_counter = 0;
static uint32_t rng32() { rng_state = rng_state * 1664525u + 1013904223u; return rng_state; }

struct Scenario {
    const char *name;
    unsigned rtt;
    unsigned jitter;
    unsigned loss;
    bool burst;
    unsigned target_frames;
    unsigned timeout_ms;
};

struct Stats {
    unsigned queued;
    unsigned dropped;
    unsigned duplicated;
    unsigned delivered;
    unsigned relayed;
    unsigned framesets;
    unsigned client_inputs;
    unsigned max_packet;
    unsigned max_queue;
    Stats() { memset(this, 0, sizeof(*this)); }
};

typedef std::priority_queue<Message, std::vector<Message>, LaterFirst> NetQueue;

static bool in_burst(unsigned now, bool burst) {
    // 900 ms total blackout, long enough to exceed the ordinary 24-frame tail.
    return burst && now >= 2500 && now < 3400;
}

static void enqueue(NetQueue &q, Stats &stats, unsigned now,
                    unsigned from, unsigned to, const uint8_t *p, int n,
                    unsigned rtt, unsigned jitter, unsigned loss, bool burst) {
    CHECK(n > 0 && n <= MAX_PACKET);
    CHECK(valid(p, n));
    if (unsigned(n) > stats.max_packet) stats.max_packet = unsigned(n);

    if (in_burst(now, burst) || (loss && (rng32() % 100u) < loss)) {
        ++stats.dropped;
        return;
    }

    int delay = int(rtt / 2u);
    if (jitter) delay += int(rng32() % (2u * jitter + 1u)) - int(jitter);
    if (delay < 0) delay = 0;

    Message m;
    m.due = now + unsigned(delay);
    m.seq = ++seq_counter;
    m.from = from;
    m.to = to;
    m.n = n;
    memcpy(m.data, p, size_t(n));
    q.push(m);
    ++stats.queued;
    if (q.size() > stats.max_queue) stats.max_queue = unsigned(q.size());

    // 5% duplicate rate to exercise duplicate/reorder tolerance.
    if ((rng32() % 20u) == 0u) {
        m.due += 3u + (rng32() % 13u);
        m.seq = ++seq_counter;
        q.push(m);
        ++stats.duplicated;
        if (q.size() > stats.max_queue) stats.max_queue = unsigned(q.size());
    }
}

static void protocol_edges_8p() {
    lobby_capacity() = 8;
    CHECK(MAX_PLAYERS == 8);
    CHECK(MAX_PACKET >= HEADER + 16 + 4 * MAX_PLAYERS * REDUNDANCY);

    BootBarrier b;
    b.reset(8);
    CHECK(!b.all_ready());
    for (unsigned s = 1; s < 8; ++s) CHECK(b.ready(s));
    CHECK(b.all_ready());
    CHECK(!b.ready(8));

    uint8_t session[16] = {0};
    uint8_t packet[MAX_PACKET] = {0};
    Stream4 host;
    host.reset(4, 8, 0);
    for (unsigned f = 0; f < REDUNDANCY; ++f) {
        for (unsigned s = 0; s < 8; ++s) {
            InputSlot &in = host.inputs[s][f % HISTORY];
            in.present = true;
            in.frame = f;
            in.pad = make_input(f, s);
        }
    }
    host.latest_complete = REDUNDANCY - 1;
    const int full = host.frameset_packet(packet, session, 7);
    CHECK(full == HEADER + 16 + 4 * 8 * REDUNDANCY);
    CHECK(full == 812);
    CHECK(valid(packet, full));

    // 4P and 8P wire signatures must reject each other.
    lobby_capacity() = 4;
    CHECK(!valid(packet, full));
    lobby_capacity() = 8;
    CHECK(valid(packet, full));

    // START validation supports slots 1..7 and 8 players.
    int n = header(packet, START, session, 5);
    packet[HEADER + 0] = 4;
    packet[HEADER + 1] = 8;
    packet[HEADER + 2] = 7;
    packet[HEADER + 3] = 0;packet[HEADER + 4] = 0;
    CHECK(valid(packet, n));

    // A forged 4P signature cannot carry an eight-player session or source slot.
    lobby_capacity()=4;
    n=header(packet,START,session,5);
    packet[HEADER]=4;packet[HEADER+1]=8;packet[HEADER+2]=7;
    CHECK(!valid(packet,n));
    n=header(packet,OFFER,session,24);packet[HEADER+20]=7;
    CHECK(!valid(packet,n));
    n=header(packet,START_ACK,session,1);packet[HEADER]=7;
    CHECK(!valid(packet,n));
    n=header(packet,CLIENT_INPUT,session,20);packet[HEADER]=7;packet[HEADER+1]=1;
    CHECK(!valid(packet,n));
    n=header(packet,FRAMESET,session,48);packet[HEADER]=8;packet[HEADER+1]=1;
    CHECK(!valid(packet,n));
    lobby_capacity()=8;
    Pad live[MAX_PLAYERS];
    for(unsigned p=0;p<MAX_PLAYERS;++p)live[p]=make_input(22,p);
    for(unsigned count=4;count<=8;++count){
        uint32_t expected=2166136261U;
        for(unsigned p=0;p<count;++p)expected=hash_step(expected,live[p]);
        CHECK(diagnostic_step(2166136261U,live,count)==expected);
        for(unsigned p=0;p<count;++p){
            live[p].buttons^=1;
            CHECK(diagnostic_step(2166136261U,live,count)!=expected);
            live[p].buttons^=1;
        }
    }
    CHECK(diagnostic_step(17,live,1)==17);
    CHECK(diagnostic_step(17,live,9)==17);

    // Each remote slot can be received without colliding with another slot.
    Stream4 h;
    h.reset(4, 8, 0);
    for (unsigned s = 1; s < 8; ++s) {
        Stream4 c;
        c.reset(4, 8, s);
        c.sample_local(make_input(0, s), 0x12345678u);
        n = c.client_packet(packet, session, 0);
        CHECK(n > 0);
        CHECK(h.receive_remote(s, packet, n));
        CHECK(equal(h.inputs[s][4].pad, make_input(0, s)));
    }
}

static void deliver_one(NetQueue &q, Stats &stats, unsigned now,
                        Stream4 streams[MAX_PLAYERS], unsigned players,
                        unsigned rtt, unsigned jitter, unsigned loss, bool burst) {
    Message m = q.top(); q.pop();
    ++stats.delivered;
    CHECK(m.to < players);
    CHECK(valid(m.data, m.n));

    if (m.to == 0) {
        CHECK(m.from > 0 && m.from < players);
        bool accepted = streams[0].receive_remote(m.from, m.data, m.n,streams[m.from].local_count);
        CHECK(accepted);
        // Exact Xbox host behavior: immediately relay accepted guest CLIENT_INPUT
        // unchanged to every other guest.
        if (m.data[5] == CLIENT_INPUT && players > 2) {
            for (unsigned dst = 1; dst < players; ++dst) {
                if (dst == m.from || !streams[dst].players) continue;
                enqueue(q, stats, now, 0, dst, m.data, m.n, rtt, jitter, loss, burst);
                ++stats.relayed;
            }
        }
        return;
    }

    if (m.data[5] == CLIENT_INPUT) {
        unsigned source = m.data[HEADER];
        CHECK(source < players && source != m.to);
        CHECK(streams[m.to].receive_remote(source, m.data, m.n,m.data[HEADER+3]+1));
    } else if (m.data[5] == FRAMESET) {
        CHECK(streams[m.to].receive_frameset(m.data, m.n));
    } else {
        CHECK(false);
    }
}

static void run_sim(unsigned players, const Scenario &sc, unsigned seed,const unsigned *ownership=0,unsigned capacity=8) {
    CHECK(players >= 2 && players <= 8);
    lobby_capacity() = capacity;
    rng_state = seed;
    seq_counter = 0;

    Stream4 streams[MAX_PLAYERS]={0};
    uint32_t state[MAX_PLAYERS];
    uint32_t sampled[MAX_PLAYERS];
    unsigned next_tick[MAX_PLAYERS];
    unsigned last_send[MAX_PLAYERS];
    for (unsigned s = 0; s < MAX_PLAYERS; ++s) {
        state[s] = 1u;
        sampled[s] = 0xffffffffu;
        next_tick[s] = 0;
        last_send[s] = 0;
    }

    // One host RTT can vary by roughly +/- 2*jitter (two one-way legs).
    // For early relay, guest A -> host -> guest B is approximately one RTT.
    const unsigned budget = sc.rtt + 2u * sc.jitter;
    const unsigned delay = input_delay_early_relay(budget, budget);
    CHECK(delay >= 2 && delay <= MAX_DELAY);

    for (unsigned s = 0; s < players; ++s) if(!ownership||ownership[s]) streams[s].reset(delay, players, s,ownership?ownership[s]:1);

    uint8_t session[16] = {0};
    uint8_t packet[MAX_PACKET];
    NetQueue q;
    Stats stats;
    uint32_t host_sent_complete = 0xffffffffu;

    unsigned now = 0;
    for (; now < sc.timeout_ms; ++now) {
        while (!q.empty() && q.top().due <= now) {
            deliver_one(q, stats, now, streams, players, sc.rtt, sc.jitter, sc.loss, sc.burst);
        }

        bool done = true;
        for (unsigned s = 0; s < players; ++s) {
            Stream4 &st = streams[s];
            if(!st.players)continue;
            CHECK(!st.fault);
            if (st.frame < sc.target_frames) done = false;

            if (now >= next_tick[s] && st.frame < sc.target_frames && sampled[s] != st.frame) {
                Pad input[2];for(unsigned j=0;j<st.local_count;++j)input[j]=make_input(st.frame,s+j);
                st.sample_locals(input, state[s]);
                CHECK(!st.fault);
                sampled[s] = st.frame;
                last_send[s] = 0;
            }

            const bool send_due = (!last_send[s] || now - last_send[s] >= 15u);
            if (s == 0) {
                const bool completion_changed = (host_sent_complete != st.latest_complete);
                if (send_due || completion_changed) {
                    for (unsigned dst = 1; dst < players; ++dst) {
                        if(!streams[dst].players)continue;
                        int n = st.client_packet(packet, session, dst);
                        CHECK(n > 0);
                        enqueue(q, stats, now, 0, dst, packet, n, sc.rtt, sc.jitter, sc.loss, sc.burst);
                        ++stats.client_inputs;
                    }
                    for (unsigned dst = 1; dst < players; ++dst) {
                        if(!streams[dst].players)continue;
                        int n = st.frameset_packet(packet, session, dst);
                        CHECK(n > 0);
                        enqueue(q, stats, now, 0, dst, packet, n, sc.rtt, sc.jitter, sc.loss, sc.burst);
                        ++stats.framesets;
                    }
                    last_send[s] = now;
                    host_sent_complete = st.latest_complete;
                }
            } else if (send_due) {
                int n = st.client_packet(packet, session, 0);
                CHECK(n > 0);
                enqueue(q, stats, now, s, 0, packet, n, sc.rtt, sc.jitter, sc.loss, sc.burst);
                ++stats.client_inputs;
                last_send[s] = now;
            }

            if (st.frame >= sc.target_frames || now < next_tick[s]) continue;
            Pad pads[MAX_PLAYERS];
            unsigned f = st.frame;
            if (st.consume(pads)) {
                for (unsigned p = 0; p < players; ++p) {
                    Pad expected = {0,0,0};
                    if (f >= delay) expected = make_input(f - delay, p);
                    CHECK(equal(pads[p], expected));

                }
                state[s] = diagnostic_step(state[s], pads, players);
                next_tick[s] = now + 33u;
            }
        }

        if (done) break;
    }

    for (unsigned s = 0; s < players; ++s) {
        if(!streams[s].players)continue;
        CHECK(streams[s].frame == sc.target_frames);
        CHECK(state[s] == state[0]);
        CHECK(!streams[s].fault);
    }

    printf("PASS %uP %-18s delay=%u frames=%u sim=%ums packets=%u drop=%u dup=%u relay=%u maxpkt=%u maxq=%u\n",
           players, sc.name, delay, sc.target_frames, now,
           stats.queued, stats.dropped, stats.duplicated, stats.relayed,
           stats.max_packet, stats.max_queue);
}

int main() {
    protocol_edges_8p();

    const Scenario scenarios[] = {
        {"clean",             0,   0,  0, false, 5000, 220000},
        {"coast-to-coast",   80, 20,  2, false, 5000, 260000},
        {"rough-wan",        180, 60,  5, false, 5000, 320000},
        {"lossy-wan",        220, 70, 10, false, 3000, 320000},
        {"900ms-outage",      80, 30,  5, true,  3000, 320000},
    };

    for (unsigned players = 4; players <= 8; ++players) {
        for (unsigned i = 0; i < sizeof(scenarios)/sizeof(scenarios[0]); ++i) {
            run_sim(players, scenarios[i], 0x4d4b3600u + players * 101u + i * 977u);
        }
    }

    printf("PASS ALL 4-8 PLAYER NETPLAY STRESS TESTS (%u checks)\n", checks);
    return 0;
}
