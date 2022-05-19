CREATE DATABASE IF NOT EXISTS lab4;
USE lab4;

CREATE TABLE IF NOT EXISTS clients (
    client_name VARCHAR(250),
    connected_at BIGINT
    );

CREATE TABLE IF NOT EXISTS events (
    client_name VARCHAR(250),
    event_type SMALLINT,
    event_time BIGINT,
    payload TEXT
    );