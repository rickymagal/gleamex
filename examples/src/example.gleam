import gleam/io
import gleamex

pub fn main() {
  let regex_pattern = "(a|b)*c"
  let test_string = "ababac"
  io.println("Regex: " <> regex_pattern)
  io.println("String: " <>test_string)

  let match_result = gleamex.match(regex_pattern, test_string)
  io.print("Match result: ")
  io.debug(match_result)

  let search_result = gleamex.search(regex_pattern, test_string, 0, 4)
  io.print("Search result (from 0 to 4): ")
  io.debug(search_result)
}
