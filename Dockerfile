FROM python:3.12-slim

RUN apt-get update \
    && apt-get install -y --no-install-recommends g++ \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN pip install --no-cache-dir Flask flask-cors gunicorn

RUN g++ /app/cpp/minipro.cpp -o /app/cpp/satellite.exe

EXPOSE 10000

CMD ["gunicorn", "--bind", "0.0.0.0:10000", "backend.app:app"]