


var token0 = S$.symbol(token0, "");
var studentId0 = S$.symbol(studentId0, "");
S$.assume(
  AND(
    OR(
      equals(studentId0, getId(mapAcess(token0, T))),
      AND(
        inMapVerify(U, studentId0),
        inList(ADMIN, getRoles(mapAcess(token0, T))),
      ),
    ),
    AND(inMapVerify(U, getId(mapAcess(token0, T))), inMapVerify(token0, T)),
  ),
);
var result1 = fetchData("http://localhost:5000/getStudent", {
  arg1: 22,
  arg2: 22,
  arg3: 22,
});
S$.assert(equals(studentId0, getId(studentData)));
