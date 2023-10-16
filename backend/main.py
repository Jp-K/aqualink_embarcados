from fastapi import FastAPI, Request
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI()

app.mount('/static', StaticFiles(directory='static', html=True), name='static')

templates = Jinja2Templates(directory="templates")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get('/')
def index():
    return 'See /{id}'

@app.get("/{id}", response_class=HTMLResponse)
async def read_item(request: Request, id: str):

    return templates.TemplateResponse("./page.html", {"request": request, "id": id})