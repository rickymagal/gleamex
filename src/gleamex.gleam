
@external(erlang, "gleamexnif", "match")
pub fn match(regex_pattern: String, test_string: String) -> Bool

@external(erlang, "gleamexnif", "search")
pub fn search(regex_pattern: String, test_string: String, first: Int, last: Int) -> Bool

