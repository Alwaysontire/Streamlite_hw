from typing import Union
from fastapi import FastAPI
import uvicorn

app = FastAPI()

@app.get("/")
def root():
    return {"message": "Hello World"}

@app.get("/items/{item_id}")
def read_item(item_id : int, q : Union[str, None] = None):
    return {"item_id" : item_id, "q" : q}

if __name__ == "__main__":
    uvicorn.run("self:app", reload=True, host="0.0.0.0", log_level="warning")