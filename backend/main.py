from fastapi import FastAPI, Request
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates
from fastapi.middleware.cors import CORSMiddleware
from typing import Annotated
import requests

app = FastAPI()

app.mount('/static', StaticFiles(directory='backend/static', html=True), name='static')

templates = Jinja2Templates(directory="backend/templates")

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

@app.get('/get/temperatura/')
def index():
    response = requests.get('http://192.168.154.241/get')
    temperatura_luminosidade = response.content.decode("utf-8").split(',')
    return temperatura_luminosidade[0]

@app.get('/get/luminosidade/')
def index():
    response = requests.get('http://192.168.154.241/get')
    temperatura_luminosidade = response.content.decode("utf-8").split(',')
    return temperatura_luminosidade[1]

@app.get('/set/temperatura/')
def index():
    response = requests.get('http://192.168.154.241/get')
    temperatura_luminosidade = response.content.decode("utf-8").split(',')
    value_to_return = '0'
    if temperatura_luminosidade[2] == '0':
        requests.get('http://192.168.154.241/res2on')
        value_to_return = '1'
    else:
        requests.get('http://192.168.154.241/res2off')
    return value_to_return

@app.get('/set/luminosidade/')
def index():
    response = requests.get('http://192.168.154.241/get')
    temperatura_luminosidade = response.content.decode("utf-8").split(',')
    value_to_return = '0'
    if temperatura_luminosidade[3] == '0':
        requests.get('http://192.168.154.241/led2on')
        value_to_return = '1'
    else:
        requests.get('http://192.168.154.241/led2off')
    return value_to_return

@app.get("/{id}", response_class=HTMLResponse)
async def read_item(request: Request, id: str):
    print(request.headers.get('host'))
    return templates.TemplateResponse("./page.html", {"request": request, "tempAnterior": id, "host": request.headers.get('host')})