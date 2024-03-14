import gleam/dict.{type Dict}
import gleam/int
import gleam/iterator.{type Iterator}
import gleam/list
import gleam/set.{type Set}
import gleam/string

type Pos {
  Pos(x: Int, y: Int)
}

fn distance(p1: Pos, p2: Pos) -> Int {
  int.absolute_value(p1.x - p2.x) + int.absolute_value(p1.y - p2.y)
}

fn parse_input(input: String) -> #(String, String) {
  let wires =
    string.split(input, "\n")
    |> list.map(string.trim)
  case wires {
    [a, b] -> #(a, b)
    _ -> panic
  }
}

fn parse_segment(segment: String, pos: Pos) -> Iterator(Pos) {
  let length =
    string.drop_left(segment, 1)
    |> int.parse
    |> unwrap_panic
  case
    string.first(segment)
    |> unwrap_panic
  {
    "U" -> iterator.iterate(pos, fn(p) { Pos(..p, y: p.y + 1) })
    "R" -> iterator.iterate(pos, fn(p) { Pos(..p, x: p.x + 1) })
    "D" -> iterator.iterate(pos, fn(p) { Pos(..p, y: p.y - 1) })
    "L" -> iterator.iterate(pos, fn(p) { Pos(..p, x: p.x - 1) })
    _ -> panic
  }
  |> iterator.drop(1)
  |> iterator.take(length)
}

fn to_points(line: String) -> Set(Pos) {
  {
    string.split(line, ",")
    |> list.fold(#(set.new(), Pos(0, 0)), fn(acc, segment) {
      parse_segment(segment, acc.1)
      |> iterator.fold(acc, fn(acc, pos) {
        let points = acc.0
        #(set.insert(points, pos), pos)
      })
    })
  }.0
}

fn unwrap_panic(result: Result(a, b)) -> a {
  case result {
    Ok(x) -> x
    Error(_) -> panic
  }
}

pub fn part1(input: String) -> Int {
  let #(w1, w2) = parse_input(input)
  let #(w1, w2) = #(to_points(w1), to_points(w2))
  set.intersection(w1, w2)
  |> set.to_list
  |> list.map(distance(_, Pos(0, 0)))
  |> list.reduce(int.min)
  |> unwrap_panic
}

pub fn part2(input: String) -> Int {
  let #(w1, w2) = parse_input(input)
  let #(w1, w2) = #(to_points2(w1), to_points2(w2))
  dict.take(w1, dict.keys(w2))
  |> dict.keys
  |> list.map(fn(p) {
    case dict.get(w1, p), dict.get(w2, p) {
      Ok(d1), Ok(d2) -> d1 + d2
      _, _ -> panic
    }
  })
  |> list.reduce(int.min)
  |> unwrap_panic
}

fn to_points2(line: String) -> Dict(Pos, Int) {
  {
    string.split(line, ",")
    |> list.fold(#(dict.new(), Pos(0, 0), 0), fn(acc, segment) {
      parse_segment(segment, acc.1)
      |> iterator.fold(acc, fn(acc, pos) {
        let points = acc.0
        let d = acc.2 + 1
        #(
          case dict.has_key(points, pos) {
            True -> points
            False -> dict.insert(points, pos, d)
          },
          pos,
          d,
        )
      })
    })
  }.0
}
