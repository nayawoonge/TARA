# TARA — 차량 내부 네트워크(IVN) 위협 분석 및 위험 평가 도구

DBC 데이터베이스로 정의된 차량 CAN 네트워크를 입력받아, **ISO/SAE 21434**의 TARA(Threat Analysis and Risk Assessment, 위협 분석 및 위험 평가) 절차를 반자동으로 수행하는 C 기반 도구 모음입니다.

DBC 파싱 → ECU 네트워크 다이어그램 생성 → 자산 식별 → STRIDE 위협 시나리오 도출 → 영향 평가 → 공격 실현 가능성 평가 → 위험도 산정의 흐름을 단계별 CSV 산출물로 남깁니다. CAN 버스 실시간 모니터링, 트레이스 로그로부터 DBC 생성, CRC 계산 등 부속 유틸리티도 포함합니다.

---

## 핵심 개념

차량 내부 네트워크(IVN)에서 ECU·CAN 메시지·시그널을 **자산(asset)** 으로 보고, 각 자산에 대해 다음 7단계 TARA를 수행합니다.

| 단계 | 내용 | 구현 |
|------|------|------|
| 1. 자산 식별 | ECU / CAN 메시지 / 시그널을 자산으로 추출 | `identify_asset()` |
| 2. 위협 시나리오 식별 | 자산별 STRIDE 위협 유형 입력 | `stride_asset()` |
| 3. 영향 평가 | Safety / Financial / Operational / Privacy(SFOP) 4축 점수(1~4) | `rating_asset()` |
| 4. 공격 경로 분석 | (attack tree, 미구현) | — |
| 5. 공격 실현 가능성 평가 | 공격 난이도 점수(1~4) 입력 | `attack_vector()` |
| 6. 위험도 산정 | SFOP + 실현가능성 합산 → 위험 등급 산출 | `risk_determination()` |
| 7. 위험 처리 결정 | 4가지 위험 처리 옵션 선택 (미구현) | — |

**STRIDE**: Spoofing / Tampering / Repudiation / Information Disclosure / Denial of Service / Elevation of Privilege

**위험 등급 산정** (`risk_det.c`) — SFOP 4점수와 공격 실현 가능성 점수를 합산:

| 합계 | 위험 등급 |
|------|-----------|
| 15–16 | Very High |
| 12–14 | High |
| 8–11 | Medium |
| 5–7 | Low |
| ≤ 4 | Very Low |

각 자산 행은 선두 구분자로 종류를 표시합니다 — `0`: ECU, `1`: CAN 메시지, `2`: 시그널.

---

## 디렉토리 구성

| 디렉토리 | 설명 |
|----------|------|
| **`ReadDbc_copy/`** | **메인 TARA 파이프라인.** DBC 파싱부터 위험도 산정까지 7단계를 모듈 단위로 구현한 최신 버전 |
| `ReadDbc/` | TARA 파이프라인 초기 버전 (자산 식별 중심) |
| `makeDBC/` | PCAN `.trc` 트레이스 로그를 분석해 DBC 파일을 생성 |
| `can_viewer/` | SocketCAN + ncurses 기반 실시간 CAN 버스 모니터 (htop 스타일) |
| `can_receive/` | SocketCAN raw 소켓으로 CAN 프레임을 타임스탬프와 함께 수신하는 최소 예제 |
| `IVN_Diagram/` | Graphviz(cgraph)로 차량 내부 네트워크 다이어그램을 그리는 실험 코드 |
| `CRC_8/` | CRC-8 SAE J1850 체크섬 계산기 (Python) |

### 메인 파이프라인 구조 (`ReadDbc_copy/`)

```
ReadDbc_copy/
├── src/main.c                  # 7단계 TARA 오케스트레이션
├── lib/
│   ├── util.h                  # CAN_Message / CAN_Signal / ECU 구조체 정의
│   ├── parse_dbc_file.c        # DBC 파싱 (BU_ / BO_ / SG_ 섹션)
│   ├── struct_print.c          # 파싱 결과 디버그 출력
│   ├── ecu_diagram.c           # ECU 네트워크 .dot/.svg 다이어그램 생성 (graphviz)
│   ├── stride.c                # STRIDE 위협 시나리오 입력
│   ├── impact_rating.c         # SFOP 영향 평가 입력
│   ├── attack_feasibility.c    # 공격 실현 가능성 평가 입력
│   └── risk_det.c              # 위험 등급 산정
├── data/
│   ├── dbc/                    # 입력 DBC (turn_light.dbc, Passenger_Cars_V1.9.2.dbc)
│   ├── dot/                    # 메시지별 ECU 다이어그램 (.dot/.svg)
│   └── csv/<dbc명>/            # 단계별 산출 CSV
└── Makefile
```

---

## 빌드 및 실행

### 사전 요구사항

- GCC, GNU Make
- **Graphviz 개발 라이브러리** (`libcgraph`, `libgvc`) — ECU 다이어그램 생성용
- **ncurses** — `can_viewer` 사용 시
- **SocketCAN** 환경 (`can_viewer`, `can_receive` 사용 시)

```bash
# Debian/Ubuntu 예시
sudo apt install build-essential libgraphviz-dev libncurses-dev can-utils
```

### 메인 TARA 파이프라인

```bash
cd ReadDbc_copy
make            # libcgraph, libgvc 링크
./ReadDbc
```

실행하면 콘솔에서 ECU / 메시지 / 시그널마다 STRIDE 유형, SFOP 점수, 공격 실현 가능성 점수를 대화식으로 입력받고, `data/csv/<dbc명>/` 아래에 다음 산출물을 단계적으로 생성합니다.

```
asset_identify.csv      # 1. 자산 식별
asset_stride.csv        # 2. 위협 시나리오
asset_rating.csv        # 3. 영향 평가 (+SFOP)
attack_feasibility.csv  # 5. 공격 실현 가능성 (+점수)
risk_determination.csv  # 6. 위험도 (+합계 +등급)
```

동시에 `data/dot/<dbc명>/`에 메시지별 ECU 송수신 관계 다이어그램(`.dot`, `.svg`)이 생성됩니다.

### 부속 도구

```bash
# CAN 버스 실시간 모니터 (ncurses)
cd can_viewer && gcc src/main.c -lncurses -o can_viewer && ./can_viewer

# 트레이스 로그 → DBC 생성
cd makeDBC && gcc src/main.c -o makeDBC && ./makeDBC data/B-CAN.trc

# CRC-8 (SAE J1850) 계산
python3 CRC_8/CRC.py
```

---

## ⚠️ 알려진 제약 사항

- **하드코딩된 절대 경로**: `ReadDbc_copy/src/main.c`, `makeDBC/src/main.c` 등에 `/home/lisa/TARA/...` 형태의 경로가 박혀 있습니다. 다른 환경에서 실행하려면 소스의 경로를 본인 환경에 맞게 수정해야 합니다.
- **네트워크 인터페이스 고정**: `can_viewer`, `can_receive`는 인터페이스 이름이 `can1`로 하드코딩되어 있습니다.
- **미구현 단계**: 4단계(공격 경로 분석, attack tree)와 7단계(위험 처리 결정)는 자리만 표시되어 있고 구현되지 않았습니다. 버스 모니터링 연동 부분도 자리표시 상태입니다.
- **입력 방식**: STRIDE·점수 평가는 모두 표준 입력(stdin) 대화식으로 받으므로, 자산 수가 많으면 입력량이 큽니다.

---

## 라이선스

별도 명시 없음 (연구/학습용).
