#ifndef MK64_UPNP_H
#define MK64_UPNP_H
/* Optional leased IGD mapping. Only the discovered private LAN device is
 * contacted; no redirect, script, DNS name or arbitrary control host is used. */
static sockaddr_in upnp_device;
static char upnp_path[512],upnp_service[96];
static bool upnp_mapped;
static bool private_ip(uint32_t ip){return (ip>>24)==10||(ip>>20)==0xAC1||(ip>>16)==0xC0A8;}
static bool safe_text(const char *s){for(;*s;++s)if((unsigned char)*s<32||*s=='"'||*s=='<'||*s=='>')return false;return true;}
static bool url(const char *s,sockaddr_in &addr,char *path,unsigned cap) {
    if(strncmp(s,"http://",7))return false;s+=7;const char *slash=strchr(s,'/');if(!slash||slash-s>=64||strlen(slash)>=cap||!safe_text(slash))return false;
    char endpoint[64];memcpy(endpoint,s,slash-s);endpoint[slash-s]=0;uint32_t ip;uint16_t port;
    if(!mknet::parse_endpoint(endpoint,ip,port)||!private_ip(ip))return false;if(!strchr(endpoint,':'))port=80;
    memset(&addr,0,sizeof(addr));addr.sin_family=AF_INET;addr.sin_addr.s_addr=htonl(ip);addr.sin_port=htons(port);strcpy(path,slash);return true;
}
static bool xml_value(const char *xml,const char *tag,char *out,unsigned cap) {
    char a[100],b[100];_snprintf(a,sizeof(a),"<%s>",tag);_snprintf(b,sizeof(b),"</%s>",tag);
    const char *p=strstr(xml,a);if(!p)return false;p+=strlen(a);const char *end=strstr(p,b);if(!end||unsigned(end-p)>=cap)return false;
    memcpy(out,p,end-p);out[end-p]=0;return safe_text(out);
}
static bool http(const sockaddr_in &to,const char *request,char *response,int cap) {
    SOCKET s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);if(s==INVALID_SOCKET)return false;u_long nb=1;ioctlsocket(s,FIONBIO,&nb);
    int result=connect(s,(const sockaddr*)&to,sizeof(to));if(result==SOCKET_ERROR&&WSAGetLastError()!=WSAEWOULDBLOCK){closesocket(s);return false;}
    DWORD start=GetTickCount();int sent=0,n=(int)strlen(request),got=0;bool connected=result==0;
    while(GetTickCount()-start<3000){
        if(!connected){fd_set w,e;FD_ZERO(&w);FD_ZERO(&e);FD_SET(s,&w);FD_SET(s,&e);timeval tv={0,0};select(0,0,&w,&e,&tv);if(FD_ISSET(s,&e))break;if(!FD_ISSET(s,&w)){Sleep(5);continue;}connected=true;}
        if(sent<n){int k=send(s,request+sent,n-sent,0);if(k>0)sent+=k;else if(k==0||WSAGetLastError()!=WSAEWOULDBLOCK)break;}
        if(sent==n){int k=recv(s,response+got,cap-1-got,0);if(k>0){got+=k;response[got]=0;if(got==cap-1)break;}else if(k==0){closesocket(s);return got>=12&&(!strncmp(response,"HTTP/1.1 200",12)||!strncmp(response,"HTTP/1.0 200",12));}else if(WSAGetLastError()!=WSAEWOULDBLOCK)break;}
        Sleep(5);
    }closesocket(s);return false;
}
static bool soap(const char *action,const char *args,char *response,int cap) {
    char body[2048],request[3072];uint32_t ip=ntohl(upnp_device.sin_addr.s_addr);
    int n=_snprintf(body,sizeof(body),"<?xml version=\"1.0\"?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:%s xmlns:u=\"%s\">%s</u:%s></s:Body></s:Envelope>",action,upnp_service,args,action);
    if(n<0||n>=sizeof(body))return false;
    n=_snprintf(request,sizeof(request),"POST %s HTTP/1.1\r\nHost: %u.%u.%u.%u:%u\r\nContent-Type: text/xml; charset=\"utf-8\"\r\nSOAPAction: \"%s#%s\"\r\nContent-Length: %u\r\nConnection: close\r\n\r\n%s",upnp_path,ip>>24,(ip>>16)&255,(ip>>8)&255,ip&255,ntohs(upnp_device.sin_port),upnp_service,action,(unsigned)strlen(body),body);
    return n>0&&n<sizeof(request)&&http(upnp_device,request,response,cap);
}
static void unmap_router() {
    if(!upnp_mapped)return;upnp_mapped=false;char response[4096];
    soap("DeletePortMapping","<NewRemoteHost></NewRemoteHost><NewExternalPort>6464</NewExternalPort><NewProtocol>UDP</NewProtocol>",response,sizeof(response));
}
static bool map_router(char *public_ip_text) {
    XNADDR title;memset(&title,0,sizeof(title));XNetGetTitleXnAddr(&title);if(!title.ina.s_addr)return false;
    SOCKET discovery=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);if(discovery==INVALID_SOCKET)return false;
    u_long nb=1;ioctlsocket(discovery,FIONBIO,&nb);sockaddr_in dest;memset(&dest,0,sizeof(dest));dest.sin_family=AF_INET;dest.sin_port=htons(1900);dest.sin_addr.s_addr=htonl(0xEFFFFFFA);
    const char *search="M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: \"ssdp:discover\"\r\nMX: 1\r\nST: urn:schemas-upnp-org:device:InternetGatewayDevice:1\r\n\r\n";
    sendto(discovery,search,(int)strlen(search),0,(sockaddr*)&dest,sizeof(dest));
    DWORD begin=GetTickCount();char location[512]={0};sockaddr_in source;
    while(GetTickCount()-begin<1800){char p[2048];int len=sizeof(source);int n=recvfrom(discovery,p,sizeof(p)-1,0,(sockaddr*)&source,&len);
        if(n>0&&private_ip(ntohl(source.sin_addr.s_addr))){p[n]=0;char *line=p;while(line&&*line){if(!_strnicmp(line,"location:",9)){line+=9;while(*line==' ')++line;char *end=strstr(line,"\r\n");if(end&&end-line<sizeof(location)){memcpy(location,line,end-line);location[end-line]=0;}break;}char *next=strstr(line,"\r\n");line=next?next+2:0;}if(location[0])break;}Sleep(10);
    }closesocket(discovery);if(!location[0])return false;
    sockaddr_in description;char path[512];if(!url(location,description,path,sizeof(path))||description.sin_addr.s_addr!=source.sin_addr.s_addr)return false;
    uint32_t ip=ntohl(description.sin_addr.s_addr);char request[1024];static char response[32768];
    _snprintf(request,sizeof(request),"GET %s HTTP/1.1\r\nHost: %u.%u.%u.%u:%u\r\nConnection: close\r\n\r\n",path,ip>>24,(ip>>16)&255,(ip>>8)&255,ip&255,ntohs(description.sin_port));
    if(!http(description,request,response,sizeof(response)))return false;
    char *service=response;bool found=false;
    while((service=strstr(service,"<service>"))!=0){char *end=strstr(service,"</service>");if(!end)break;char saved=*end;*end=0;char type[96],control[512];
        bool match=xml_value(service,"serviceType",type,sizeof(type))&&(!strcmp(type,"urn:schemas-upnp-org:service:WANIPConnection:1")||!strcmp(type,"urn:schemas-upnp-org:service:WANIPConnection:2")||!strcmp(type,"urn:schemas-upnp-org:service:WANPPPConnection:1"))&&xml_value(service,"controlURL",control,sizeof(control));*end=saved;
        if(match){upnp_device=description;strcpy(upnp_service,type);
            if(!strncmp(control,"http://",7)){if(!url(control,upnp_device,upnp_path,sizeof(upnp_path))||upnp_device.sin_addr.s_addr!=description.sin_addr.s_addr)return false;}
            else if(control[0]=='/'){strcpy(upnp_path,control);}
            else{char *last=strrchr(path,'/');if(!last||strlen(control)+size_t(last-path)+2>=sizeof(upnp_path))return false;memcpy(upnp_path,path,last-path+1);strcpy(upnp_path+(last-path+1),control);}
            found=true;break;}service=end+10;
    }if(!found)return false;
    char args[640];ip=ntohl(title.ina.s_addr);
    _snprintf(args,sizeof(args),"<NewRemoteHost></NewRemoteHost><NewExternalPort>6464</NewExternalPort><NewProtocol>UDP</NewProtocol><NewInternalPort>6464</NewInternalPort><NewInternalClient>%u.%u.%u.%u</NewInternalClient><NewEnabled>1</NewEnabled><NewPortMappingDescription>MK64 B19</NewPortMappingDescription><NewLeaseDuration>86400</NewLeaseDuration>",ip>>24,(ip>>16)&255,(ip>>8)&255,ip&255);
    if(!soap("AddPortMapping",args,response,sizeof(response)))return false;upnp_mapped=true;
    if(soap("GetExternalIPAddress","",response,sizeof(response))){char external[32];uint16_t ignored;if(xml_value(response,"NewExternalIPAddress",external,sizeof(external))&&mknet::parse_endpoint(external,ip,ignored)&&!private_ip(ip)&&(ip>>22)!=0x191&&(ip>>24)!=127&&(ip>>16)!=0xA9FE){address_text(public_ip_text,ip,6464);}}
    return true;
}
#endif
