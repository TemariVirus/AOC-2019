import day01
import gleam/int
import gleam/io
import simplifile

pub fn main() {
  let input = case simplifile.read("src/day01.txt") {
    Ok(s) -> s
    Error(_) -> panic as "Failed to read input!"
  }
  time("Part 1: ", fn() { day01.part1(input) })
  io.println("")
  time("Part 2: ", fn() { day01.part2(input) })
}

fn time(label: String, func: fn() -> Int) {
  let start = timestamp()
  let result = func()
  let time = timestamp() - start
  io.println(label <> int.to_string(result))
  io.println("Time: " <> int.to_string(time) <> "ms")
}

/// Returns a timestamp in milliseconds
fn timestamp() -> Int {
  let #(mega, sec, micro) = erlang_timestamp()
  { { mega * 1_000_000 + sec } * 1_000_000 + micro } / 1000
}

@external(erlang, "os", "timestamp")
fn erlang_timestamp() -> #(Int, Int, Int)
