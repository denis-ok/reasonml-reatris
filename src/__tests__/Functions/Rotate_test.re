open Jest;
open Expect;
open Types;
open Core

let blockR1 =
[|
[|X, X, X|],
[|O, X, O|],
|];

let blockR2 =
[|
[|O, X|],
[|X, X|],
[|O, X|],
|];

let m1 =
[|
[|1, 2, 3|],
[|4, 5, 6|],
[|7, 8, 9|],
|];

let m2 =
[|
[|7, 4, 1|],
[|8, 5, 2|],
[|9, 6, 3|],
|];

let m3 =
[|
[|9, 8, 7|],
[|6, 5, 4|],
[|3, 2, 1|],
|];

let m4 =
[|
[|3, 6, 9|],
[|2, 5, 8|],
[|1, 4, 7|],
|];


let () =
  describe("Rotate block", () => {
    test("Should rotate m1 to m2", () => {
      let result = rotateClockwise(m1);
      expect(result) |> toEqual(m2);
    });

    test("Should rotate m2 to m3", () => {
      let result = rotateClockwise(m2);
      expect(result) |> toEqual(m3);
    });

    test("Should rotate m3 to m4", () => {
      let result = rotateClockwise(m3);
      expect(result) |> toEqual(m4);
    });

    test("Should rotate m4 to m1", () => {
      let result = rotateClockwise(m4);
      expect(result) |> toEqual(m1);
    });

    test("Should rotate BlockR1 to BlockR2", () => {
      let result = rotateClockwise(blockR1);
      expect(result) |> toEqual(blockR2);
    });

    test("Should fully rotate Block", () => {
      let result = blockR1
        |> rotateClockwise
        |> rotateClockwise
        |> rotateClockwise
        |> rotateClockwise;

      expect(result) |> toEqual(blockR1);
    });
  });
