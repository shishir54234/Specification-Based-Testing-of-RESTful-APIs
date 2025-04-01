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
getStudent(studentId0, token0);
S$.assert(equals(studentId0, getId(studentData)));