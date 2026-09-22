# MKart360 — Fork com Texturas HD

Fork de [sirdankz/MKart360](https://github.com/sirdankz/MKart360) (porte de Mario
Kart 64 para Xbox 360, baseado na decompilação [n64decomp/mk64](https://github.com/n64decomp/mk64)).
Este fork trabalha **somente com a ROM US** e adiciona um sistema de
**substituição de texturas em HD em tempo de execução**, sem alterar a ROM
nem os assets compilados do jogo.

---

## O que mudou em relação ao projeto original

### Código alterado

- **`mk64-master/include/xbox360/gfx_pc.c`**
  Gancho de textura HD dentro de `import_texture()`: o jogo calcula um hash
  FNV-1a do conteúdo da textura original e, se existir uma versão HD com
  aquele hash, ela é usada no lugar. Inclui:
  - leitura do `tex.pak` com índice em tabela hash;
  - fallback para arquivos `.tex` avulsos em subpastas (para contornar o
    limite de arquivos por pasta do FATX);
  - cache das texturas HD em RAM (evita reler do disco);
  - cache de texturas ampliado (de 512 para 1024 entradas);
  - trace de diagnóstico desligado por padrão (`X360_HDTEX_TRACE 0`).
  - `x360_try_draw_hd_menu_quad` já está presente no código, mas **inativa**
    (ainda não é chamada) — reservada para uma futura substituição das
    texturas grandes de menu.

- **`mk64-master/src/xbox360/xbox360_renderer.cpp`**
  Passa a reaproveitar a textura do Direct3D quando o tamanho não muda,
  evitando `CreateTexture`/`Release` a cada frame enviado — reduz o
  overhead ao trocar texturas com frequência.

- **`mk64-master/PUBLIC_SOURCE_GOLD_HASHES.json`**
  Hash de verificação do renderer atualizado para refletir a mudança acima.

- **`mk64-master/.gitignore`**
  Passa a ignorar as texturas extraídas, o `tex.pak`, os logs de trace e
  `src/xbox360/generated_banks/` (tudo derivado da ROM do usuário, não deve
  ir para o controle de versão).

### O que ainda não foi resolvido (conhecido)

- As imagens grandes de menu (formato **TKMK00**, tela inicial, nomes de
  personagem) ainda **não** são substituídas.
- As janelas de TMEM dos sprites do Lakitu (56×72) seguem a mesma regra
  medida nos karts, sem medição própria — pode não estar 100% correta.

---

## Ferramentas novas

Todos os scripts abaixo ficam em `mk64-master/` e são executados de dentro
dessa pasta.

| Arquivo | Função |
|---|---|
| `EXTRACT_MK64_TEXTURES.py` | Extrai as texturas da ROM em PNGs editáveis |
| `EXTRACT_LAKITU.py` | Extrai especificamente todos os quadros do Lakitu (semáforo, bandeirada, pesca, volta final, contramão etc.) |
| `PACK_TEXTURES.py` | Empacota os PNGs editados de volta no formato que o jogo lê (`tex.pak`) |
| `HALVE_PNGS.py` | Reduz PNGs pela metade, para caber na memória do console |
| `SCAN_HALVES.py` | Ferramenta de diagnóstico: descobre onde ficam as "metades de baixo" dos sprites de kart que não bateram no hash |
| `CROSS_CHECK.py` | Cruza o log de trace do jogo com os manifests para achar texturas que não foram encontradas |

Requisito único de instalação (uma vez):

```powershell
pip install pillow
```

### Fluxo completo de uso

**1. Extrair as texturas da ROM**

```powershell
py .\EXTRACT_MK64_TEXTURES.py --rom .\baserom.us.z64
```

Gera a pasta `extracted_textures\` com:

- **raiz** — texturas comuns, nomeadas `<hash>__nome.png`;
- **`generated\`** — bancos gerados (menus, HUD), nomeados pelo símbolo;
- **`karts\<personagem>\frames\`** — sprites de piloto+kart (321 por
  personagem);
- **`*_manifest.json`** — metadados que ligam cada PNG ao seu hash. **Não
  apague nem mova esses arquivos.**

Por padrão o extrator não sobrescreve PNGs já existentes (roda de novo com
segurança, só preenche o que falta). Use `--force` para regerar tudo do
zero (descarta edições). Outras opções: `--no-karts`, `--no-generated`,
`--out PASTA`.

Para extrair só os quadros do Lakitu:

```powershell
py .\EXTRACT_LAKITU.py --rom .\baserom.us.z64
py .\EXTRACT_LAKITU.py --rom .\baserom.us.z64 --force   # sobrescreve PNGs
```

**2. Editar**

Abra os PNGs e refaça em HD. Pode aumentar a resolução à vontade (ex.:
256×256 no lugar de um 64×64 original). **Não renomeie os arquivos** — o
nome (ou o caminho registrado no manifest) é o que liga a textura editada à
original.

**3. Empacotar**

```powershell
py .\PACK_TEXTURES.py --only karts --pak
```

Gera `tex\tex.pak`, um arquivo único com tudo dentro.

| Opção | Efeito |
|---|---|
| `--pak` | Gera um arquivo só (**recomendado**) |
| `--only PREFIXO` | Limita a um subconjunto, ex.: `--only karts\bowser` |
| `--out PASTA` | Pasta de saída (padrão `tex`) |
| `--max N` | Ignora imagens maiores que N pixels (padrão 2048) |

Sem `--pak` são gerados milhares de `.tex` avulsos — funciona, mas carrega
mais devagar e é mais frágil de transferir; use só para depuração. Empacote
apenas o que você realmente editou: PNGs não editados geram texturas
idênticas às originais, sem ganho visual mas ocupando espaço e memória.

**4. Instalar no console**

Copie o `tex.pak` para a raiz da pasta do jogo, ao lado do executável:

```
MK64.xex
baserom.us.z64
tex.pak          <- aqui, NÃO dentro de uma pasta tex\
```

Trocar texturas não exige recompilar — o `tex.pak` é lido em tempo de
execução. Recompilar só é necessário ao alterar código C.

### Limites de memória

O Xbox 360 tem 512 MB compartilhados entre sistema e vídeo. O elenco
completo de personagens soma 2568 sprites (2 arquivos cada):

| Resolução | Total estimado | Observação |
|---|---|---|
| 256×256 | ~640 MB | não cabe |
| 128×128 | ~160 MB | recomendado |
| 96×96 | ~90 MB | folga maior |

Para reduzir PNGs já editados:

```powershell
py .\HALVE_PNGS.py --recursive              # prévia, não altera nada
py .\HALVE_PNGS.py --apply --recursive      # aplica de fato
```

Use `--backup` para guardar os originais como `*.orig.png`.

### Diagnóstico (quando uma textura não aparece em HD)

Em `include\xbox360\gfx_pc.c`, troque:

```c
#define X360_HDTEX_TRACE 0   →   1
```

Recompile, jogue alguns segundos e pegue o arquivo `game:\hdtex-trace.log`.
Cada linha traz o hash calculado em tempo de execução e `found=1` ou
`found=0`. Para cruzar esse log com os manifests:

```powershell
py .\CROSS_CHECK.py --log .\hdtex-trace.log --kart bowser
```

Se a textura que falta for uma "metade de baixo" de sprite de kart que
nunca bate (`found=0` mesmo existindo no manifest), rode:

```powershell
py .\SCAN_HALVES.py --rom .\baserom.us.z64 --log .\hdtex-trace.log --kart bowser
```

Isso varre o bloco descomprimido da ROM procurando qual janela de 2048
bytes produz o hash que faltou, mostrando o deslocamento real da metade —
útil para corrigir o extrator.

Deixe o trace desligado no uso normal: ele grava em disco durante o jogo e
afeta a performance.

### Problemas comuns

- **As texturas não aparecem** → confira se o `tex.pak` está na raiz, ao
  lado do `MK64.xex` (não dentro de `tex\`). Se estiver certo, force uma
  recompilação completa (`/t:Rebuild`) — o `gfx_pc.c` é incluído por outro
  arquivo e a build incremental às vezes não percebe a mudança.
- **Só metade do sprite fica em HD** → sprites CI8 são carregados em duas
  janelas por causa do limite de 4 KB da TMEM, cada uma com hash próprio.
  O extrator já trata isso; se acontecer, regenere os manifests com o
  extrator atualizado e reempacote.
- **A transferência para o console falha no fim** → o sistema de arquivos
  do Xbox 360 (FATX) aceita no máximo 4096 arquivos por pasta. É exatamente
  o que a opção `--pak` resolve, gerando um arquivo só.
- **Engasgos no jogo** → reduza a resolução (128×128) ou o número de
  personagens substituídos. Uma travadinha ao um oponente novo entrar em
  cena é esperada: os sprites dele são carregados naquele momento.

### Como funciona por baixo dos panos

1. O jogo carrega uma textura e calcula um hash FNV-1a do conteúdo
   original.
2. `gfx_pc.c` procura esse hash no `tex.pak`.
3. Se achar, envia a versão HD para a GPU no lugar da original.
4. Se não achar, segue o caminho normal — nada quebra.

As dimensões originais são preservadas internamente para o mapeamento de
coordenadas, então a textura HD pode ter qualquer resolução. Os dados lidos
ficam em cache na RAM para evitar releituras do disco.

---

## Como compilar o projeto

### Requisitos

- Windows
- Python 3
- Xbox 360 SDK / ferramentas de build para Xbox 360 do Visual Studio
- Sua própria ROM US de Mario Kart 64
- Um Xbox 360 capaz de rodar arquivos XEX homebrew

### 1. Preparar os assets

Coloque sua ROM em:

```
mk64-master\baserom.us.z64
```

Depois, dentro de `mk64-master`:

```powershell
py ".\PUBLIC_PREPARE_MK64_ASSETS.py"
```

O script verifica a ROM e gera os arquivos derivados dela que ficam de fora
do código-fonte público.

### 2. Compilar

A partir da pasta que contém `MK64.sln`:

```powershell
& "$env:WINDIR\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe" ".\MK64.sln" /t:Build "/p:Configuration=Release" "/p:Platform=Xbox 360"
```

Ou, de forma mais simples:

```powershell
powershell -ExecutionPolicy Bypass -File ".\PUBLIC_BUILD_XBOX360.ps1"
```

O `.xex` compilado fica na pasta de saída Release do projeto Xbox 360.

### 3. Aplicar as texturas HD (opcional)

Depois de compilar, gere e copie o `tex.pak` como descrito na seção
["Ferramentas novas"](#ferramentas-novas) acima. Trocar texturas depois não
exige recompilar de novo.

> Se algo não bater após editar `gfx_pc.c`, force `/t:Rebuild` — a build
> incremental às vezes não detecta a mudança nesse arquivo.

---

## Créditos

- **[n64decomp/mk64](https://github.com/n64decomp/mk64)** — decompilação
  original de Mario Kart 64, base de todo o projeto.
- **[sirdankz/MKart360](https://github.com/sirdankz/MKart360)** — porte
  para Xbox 360 (build nativo, multiplayer console-a-console, correções de
  renderização), do qual este repositório é um fork.

Mario Kart 64 e as propriedades relacionadas pertencem à Nintendo. Este é
um porte homebrew não oficial, sem afiliação com a Nintendo.
