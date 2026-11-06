# ---------- Этап 1: сборка ----------
FROM ubuntu:22.04 AS builder
RUN apt-get update && apt-get install -y --no-install-recommends cmake g++ make \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY CMakeLists.txt ./
COPY include ./include
COPY src ./src
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF && cmake --build build --target matrix_calculator

# ---------- Этап 2: минимальный образ для запуска ----------
FROM ubuntu:22.04
WORKDIR /app
COPY --from=builder /app/build/matrix_calculator .
CMD ["./matrix_calculator"]
