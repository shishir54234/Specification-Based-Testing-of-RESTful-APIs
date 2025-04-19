import {
  input,
  mapped_value,
  not_in,
  dom,
  token,
  signup,
  login,
  assume,
  assert,
  equals,
  in_dom,
} from "./jsCodeHeader.js";
const S$ = require("S$");
let U;
let T;
let username;
let password;
U = new Map([
  [alice, "alicepass"],
  [bob, "bob123"],
  [charlie, "qwerty"],
]);
T = new Map([]);
username = input();
password = input();
assume(not_in(username, dom(U)));
signup(username, password);
assert(equals(mapped_value(U, username), password));
username = input();
password = input();
assume(
  and_operator(equals(password, mapped_value(U, username)), _in(T, token))
);
login(username, password);
assert(equals(mapped_value(T, token), username));
