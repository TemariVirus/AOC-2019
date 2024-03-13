import gleam/int
import gleam/list
import gleam/string

fn parse_input(input: String) -> List(Int) {
  string.split(input, "\n")
  |> list.map(fn(x) -> Int {
    case int.parse(string.trim(x)) {
      Ok(x) -> x
      Error(_) -> panic as "Failed to parse int"
    }
  })
}

pub fn part1(input: String) -> Int {
  let inputs = parse_input(input)
  let fuels = list.map(inputs, get_fuel)
  list.fold(fuels, 0, fn(a, b) { a + b })
}

fn get_fuel(mass: Int) -> Int {
  mass / 3 - 2
}

pub fn part2(input: String) -> Int {
  let inputs = parse_input(input)
  let fuels = list.map(inputs, total_fuel)
  list.fold(fuels, 0, fn(a, b) { a + b })
}

fn total_fuel(mass: Int) -> Int {
  let partial_fuel = get_fuel(mass)
  case partial_fuel {
    x if x < 0 -> 0
    _ -> partial_fuel + total_fuel(partial_fuel)
  }
}
