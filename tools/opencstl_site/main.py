from pathlib import Path

from fastapi import FastAPI, Request
from fastapi.responses import FileResponse, HTMLResponse, PlainTextResponse, Response
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates
from slowapi import Limiter, _rate_limit_exceeded_handler
from slowapi.errors import RateLimitExceeded
from slowapi.middleware import SlowAPIMiddleware
from slowapi.util import get_remote_address

BASE_DIR = Path(__file__).resolve().parent

# Per-client-IP rate limiting to mitigate DDoS / abusive traffic.
# Default limits apply to every route via SlowAPIMiddleware.
limiter = Limiter(key_func=get_remote_address, default_limits=["120/minute", "10/second"])

app = FastAPI(title="OpenCSTL Documentation")
app.state.limiter = limiter
app.add_exception_handler(RateLimitExceeded, _rate_limit_exceeded_handler)
app.add_middleware(SlowAPIMiddleware)
app.mount("/static", StaticFiles(directory=BASE_DIR / "static"), name="static")
templates = Jinja2Templates(directory=BASE_DIR / "templates")

CONTAINERS = [
    {
        "name": "VECTOR",
        "tag": "Sequence",
        "tone": "amber",
        "desc": "Dynamic, contiguous array with O(1) random access.",
        "snippet": (
            "VECTOR(int) v = new_vector(int);\n"
            "push_back(v, 1);\n"
            "push_back(v, 2);\n"
            "push_back(v, 3);\n"
            "destroy(v);"
        ),
        "stamp": "11:03:09:52",
    },
    {
        "name": "LIST",
        "tag": "Sequence",
        "tone": "rose",
        "desc": "Doubly linked list with O(1) insert/erase anywhere.",
        "snippet": (
            "LIST(int) lst = new_list(int);\n"
            "push_back(lst, 10);\n"
            "push_front(lst, 5);\n"
            "for (int* it = begin(lst);\n"
            "     it != end(lst);\n"
            "     it = next(it)) { ... }"
        ),
        "stamp": "11:03:09:52",
    },
    {
        "name": "MAP",
        "tag": "Associative",
        "tone": "emerald",
        "desc": "Sorted key->value tree with O(log n) lookup.",
        "snippet": (
            "MAP(int, char*) m = new_map(int, char*);\n"
            "insert(m, 1, \"one\");\n"
            "insert(m, 2, \"two\");\n"
            "char** p = at(m, 1);"
        ),
        "stamp": "11:03:09:52",
    },
]

QUICK_REF = [
    {"name": "push_back", "container": "VECTOR, LIST, DEQUE", "big": "O(1)*",    "small": "amortized",    "tone": "lime",  "stamp": "11:03:09:52"},
    {"name": "insert",    "container": "SET, MAP, U_SET",     "big": "O(log n)", "small": "tree balance", "tone": "amber", "stamp": "11:03:09:52"},
    {"name": "find",      "container": "UNORDERED_MAP",       "big": "O(1) avg", "small": "hash lookup",  "tone": "sky",   "stamp": "11:03:09:52"},
    {"name": "pop_front", "container": "DEQUE, QUEUE, LIST",  "big": "O(1)",     "small": "constant",     "tone": "rose",  "stamp": "11:03:09:52"},
]

TONE_BG = {
    "amber":   "bg-amber-100 text-amber-700 ring-amber-200",
    "rose":    "bg-rose-100 text-rose-700 ring-rose-200",
    "emerald": "bg-emerald-100 text-emerald-700 ring-emerald-200",
    "lime":    "bg-lime-100 text-lime-700 ring-lime-200",
    "sky":     "bg-sky-100 text-sky-700 ring-sky-200",
}

TABS = ["Containers", "Functions", "Examples", "Headers"]

COMPILERS = [
    {"name": "MSVC",  "color": "bg-sky-500",     "supports": ["win"]},
    {"name": "GCC",   "color": "bg-amber-500",   "supports": ["win", "mac", "linux"]},
    {"name": "Clang", "color": "bg-orange-500",  "supports": ["win", "mac", "linux"]},
    {"name": "TCC",   "color": "bg-emerald-500", "supports": ["win", "mac", "linux"]},
]

COMPONENTS = ["vector", "list", "deque", "set", "map", "stack", "queue", "pqueue", "u_set", "u_map"]

PLATFORMS = ["win", "mac", "linux"]


@app.get("/", response_class=HTMLResponse)
async def index(request: Request) -> HTMLResponse:
    return templates.TemplateResponse(
        request,
        "index.html",
        {
            "containers": CONTAINERS,
            "quick_ref": QUICK_REF,
            "tone_bg": TONE_BG,
            "tabs": TABS,
            "compilers": COMPILERS,
            "components": COMPONENTS,
            "platforms": PLATFORMS,
        },
    )


@app.get("/favicon.svg", include_in_schema=False)
async def favicon() -> FileResponse:
    return FileResponse(BASE_DIR / "static" / "favicon.svg", media_type="image/svg+xml")


@app.get("/icons.svg", include_in_schema=False)
async def icons() -> FileResponse:
    return FileResponse(BASE_DIR / "static" / "icons.svg", media_type="image/svg+xml")


@app.get("/robots.txt", include_in_schema=False)
async def robots(request: Request) -> PlainTextResponse:
    # Allow all crawlers full access and advertise the sitemap.
    sitemap_url = str(request.base_url).rstrip("/") + "/sitemap.xml"
    body = f"User-agent: *\nDisallow:\nSitemap: {sitemap_url}\n"
    return PlainTextResponse(body)


@app.get("/sitemap.xml", include_in_schema=False)
async def sitemap(request: Request) -> Response:
    base = str(request.base_url).rstrip("/")
    urls = ["/"]
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

    uvicorn.run("main:app", host="0.0.0.0", port=80, reload=True)
