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
var token1 = S$.symbol(token1, "");
S$.assume(
  AND(
    inList(ADMIN, getRoles(mapAcess(token1, T))),
    AND(inMapVerify(U, getId(mapAcess(token1, T))), inMapVerify(token1, T)),
  ),
);
getAllStudents(token1);
S$.assert(equals(S, studentList));
var token2 = S$.symbol(token2, "");
var studentId2 = S$.symbol(studentId2, "");
S$.assume(
  AND(
    OR(
      equals(studentId2, getId(mapAcess(token2, T))),
      AND(
        inMapVerify(U, studentId2),
        inList(ADMIN, getRoles(mapAcess(token2, T))),
      ),
    ),
    AND(inMapVerify(U, getId(mapAcess(token2, T))), inMapVerify(token2, T)),
  ),
);
S == S_old;
deleteStudent(studentId2, token2);
S$.assert(equals(setSubtraction(studentId2, S_old), S));
var token3 = S$.symbol(token3, "");
var studentData3 = S$.symbol(studentData3, "");
S$.assume(
  AND(inMapVerify(U, getId(mapAcess(token3, T))), inMapVerify(token3, T)),
);
S == S_old;
saveStudent(studentData3, token3);
S$.assert(equals(Union(studentId, S_old), S));
