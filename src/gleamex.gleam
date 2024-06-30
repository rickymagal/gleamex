import gleam/io

// Definição da função externa para match
@external(erlang, "gleamexnif", "match")
pub fn match(regex_pattern: String, test_string: String) -> Bool

// Definição da função externa para search
@external(erlang, "gleamexnif", "search")
pub fn search(regex_pattern: String, test_string: String, first: Int, last: Int) -> Bool

pub fn main() {
  let regex_pattern = "(a|b)*c"
  let test_string = "ababac"

  let match_result = match(regex_pattern, test_string)
  io.debug(match_result)

  let search_result = search(regex_pattern, test_string, 0, 4)
  io.debug(search_result)
}
