import gleam/dict.{type Dict}
import gleam/int
import gleam/iterator.{type Iterator}
import gleam/list
import gleam/string

type Array =
  Dict(Int, Int)

fn parse_input(input: String) -> Array {
  let op_codes =
    string.split(input, ",")
    |> list.map(fn(x) -> Int {
      case int.parse(x) {
        Ok(x) -> x
        Error(_) -> panic as "Failed to parse int"
      }
    })
  iterator.from_list(op_codes)
  |> iterator.transform(0, fn(i, code) { iterator.Next(#(i, code), i + 1) })
  |> iterator.to_list
  |> dict.from_list
}

fn unwrap_panic(result: Result(a, b)) -> a {
  case result {
    Ok(x) -> x
    Error(_) -> panic
  }
}

pub fn part1(input: String) -> Int {
  parse_input(input)
  |> run_program(12, 2)
}

fn run_program(program: Array, in1: Int, in2: Int) -> Int {
  dict.insert(program, 1, in1)
  |> dict.insert(2, in2)
  |> run_program_inner(0)
}

fn run_program_inner(program: Array, index: Int) -> Int {
  let code =
    dict.get(program, index)
    |> unwrap_panic
  case code {
    99 ->
      dict.get(program, 0)
      |> unwrap_panic
    1 | 2 -> {
      let in1 =
        dict.get(program, index + 1)
        |> unwrap_panic
        |> dict.get(program, _)
        |> unwrap_panic
      let in2 =
        dict.get(program, index + 2)
        |> unwrap_panic
        |> dict.get(program, _)
        |> unwrap_panic
      let result = case code {
        1 -> in1 + in2
        2 -> in1 * in2
        _ -> panic
      }
      let dest =
        dict.get(program, index + 3)
        |> unwrap_panic
      let program = dict.insert(program, dest, result)
      run_program_inner(program, index + 4)
    }
    _ -> panic
  }
}

pub fn part2(input: String) -> Int {
  let program = parse_input(input)
  // Brute force
  let result =
    for(0, 100)
    |> iterator.map(fn(i) {
      let result =
        for(0, 100)
        |> iterator.map(fn(j) {
          let output = run_program(program, i, j)
          case output {
            19_690_720 -> #(True, j)
            _ -> #(False, j)
          }
        })
        |> iterator.find(fn(tuple) { tuple.0 })
      case result {
        Ok(#(True, j)) -> #(True, i, j)
        Error(_) -> #(False, 0, 0)
        _ -> panic
      }
    })
    |> iterator.find(fn(tuple) { tuple.0 })
  case result {
    Ok(#(True, i, j)) -> 100 * i + j
    _ -> panic
  }
}

fn for(start: Int, end: Int) -> Iterator(Int) {
  iterator.iterate(start, fn(x) { x + 1 })
  |> iterator.take(end - start)
}
