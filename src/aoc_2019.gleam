import gleam/io
import gleam/int

pub fn main() {
  let start = timestamp()
  io.println("Hello from aoc_2019!")
  io.println("Time: " <> int.to_string(timestamp() - start) <> "ms")
}

/// Returns the number of milliseconds since the Unix epoch (or whatever epoch Erlang uses).
fn timestamp() -> Int {
  let #(mega, sec, micro) = erlang_timestamp()
  { { mega * 1_000_000 + sec } * 1_000_000 + micro } / 1000
}

@external(erlang, "os", "timestamp")
fn erlang_timestamp() -> #(Int, Int, Int)
