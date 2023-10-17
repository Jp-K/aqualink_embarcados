from fastapi import FastAPI, Request
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates
from fastapi.middleware.cors import CORSMiddleware
from typing import Annotated

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

@app.post('/set/temperatura/{value}')
def index(value: str):
    print(value)
    return True

@app.post('/set/luminosidade/{value}')
def index(value: str):
    print(value)
    return True

@app.get("/{id}", response_class=HTMLResponse)
async def read_item(request: Request, id: str):
    print(request.headers.get('host'))
    return templates.TemplateResponse("./page.html", {"request": request, "tempAnterior": id, "host": request.headers.get('host')})