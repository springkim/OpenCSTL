# OpenCSTL — Documentation Web App

OpenCSTL 문서 사이트입니다. Python **FastAPI** + **Jinja2** 템플릿으로 서빙합니다.
Tailwind는 공식 v4 브라우저 런타임(`@tailwindcss/browser`)을 CDN으로 로드하여
원본 디자인(`@theme`의 `brand-*` 컬러 토큰 포함)을 그대로 재현합니다.
Node.js / npm / Vite 빌드 단계가 없습니다.

## 구동 방법

1. 가상환경 생성 및 활성화 (최초 1회)

   ```powershell
   python -m venv .venv
   .venv\Scripts\Activate.ps1
   ```

   macOS / Linux:

   ```bash
   python -m venv .venv
   source .venv/bin/activate
   ```

2. 의존성 설치

   ```bash
   pip install -r requirements.txt
   ```

3. 개발 서버 실행

   ```bash
   python main.py
   ```

   또는 직접 uvicorn 호출:

   ```bash
   uvicorn main:app --reload --host 127.0.0.1 --port 8000
   ```

   브라우저에서 <http://127.0.0.1:8000> 접속.

## 프로젝트 구조

```
.
├── main.py              # FastAPI 앱 (라우트 + 페이지 데이터)
├── templates/
│   └── index.html       # Jinja2 템플릿
├── static/
│   ├── favicon.svg
│   ├── icons.svg
│   └── hero.png
└── requirements.txt
```
