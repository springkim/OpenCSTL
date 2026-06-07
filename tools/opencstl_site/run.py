from pathlib import Path

from fastapi import FastAPI, Request
from fastapi.responses import FileResponse, PlainTextResponse, Response
from fastapi.staticfiles import StaticFiles
from slowapi import Limiter, _rate_limit_exceeded_handler
from slowapi.errors import RateLimitExceeded
from slowapi.middleware import SlowAPIMiddleware
from slowapi.util import get_remote_address

BASE_DIR = Path(__file__).resolve().parent

# Per-client-IP rate limiting to mitigate DDoS / abusive traffic.
# Default limits apply to every route via SlowAPIMiddleware.
limiter = Limiter(key_func=get_remote_address, default_limits=["120/minute", "10/second"])

app = FastAPI(title="OpenCSTL")
app.state.limiter = limiter
app.add_exception_handler(RateLimitExceeded, _rate_limit_exceeded_handler)
app.add_middleware(SlowAPIMiddleware)
app.mount("/static", StaticFiles(directory=BASE_DIR / "static"), name="static")


@app.get("/", include_in_schema=False)
@app.get("/index.html", include_in_schema=False)
async def index() -> FileResponse:
    return FileResponse(BASE_DIR / "index.html", media_type="text/html")


@app.get("/references.html", include_in_schema=False)
async def references() -> FileResponse:
    return FileResponse(BASE_DIR / "references.html", media_type="text/html")


@app.get("/favicon.svg", include_in_schema=False)
async def favicon() -> FileResponse:
    return FileResponse(BASE_DIR / "static" / "favicon.svg", media_type="image/svg+xml")


@app.get("/robots.txt", include_in_schema=False)
async def robots(request: Request) -> PlainTextResponse:
    # Allow all crawlers full access and advertise the sitemap.
    sitemap_url = str(request.base_url).rstrip("/") + "/sitemap.xml"
    body = f"User-agent: *\nDisallow:\nSitemap: {sitemap_url}\n"
    return PlainTextResponse(body)


@app.get("/sitemap.xml", include_in_schema=False)
async def sitemap(request: Request) -> Response:
    base = str(request.base_url).rstrip("/")
    urls = ["/", "/references.html"]
    entries = "".join(f"  <url><loc>{base}{path}</loc></url>\n" for path in urls)
    body = (
        '<?xml version="1.0" encoding="UTF-8"?>\n'
        '<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">\n'
        f"{entries}"
        "</urlset>\n"
    )
    return Response(content=body, media_type="application/xml")


if __name__ == "__main__":
    import uvicorn

    uvicorn.run("run:app", host="0.0.0.0", port=80, reload=True)
