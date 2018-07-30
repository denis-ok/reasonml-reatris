open Jest;
open Expect;
open Types;
open Functions;

let testBlock = [|
[|X, X, X|],
[|O, X, O|],
|];

let testGrid1 =[|
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|X, O, X, X, X, X|],
|];

let testGrid2 =[|
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|X, X, X, O, O, O|],
|];


let testGrid3 =[|
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, X, X, X, O, O|],
[|O, O, X, O, O, O|],
[|X, O, X, X, X, X|],
|];


let () =
  describe("Tetris basic functions tests", () => {
    describe("tick", () => {

      test("1", () => {
        let stateBefore = {
          block: testBlock,
          blockPosition: { x: 0, y: 3 },
          grid: testGrid1
        };

        let expected = {
          block: testBlock,
          blockPosition: { x: 0, y: 4 },
          grid: testGrid1
        };

        expect(tick(stateBefore)) |> toEqual(expected);
      });

      test("should return new state with after strike", () => {
        let stateBefore = {
          block: testBlock,
          blockPosition: { x: 0, y: 4 },
          grid: testGrid1
        };

        let expected = {
          block: testBlock,
          blockPosition: { x: 0, y: 0 },
          grid: testGrid2
        };

        expect(tick(stateBefore)) |> toEqual(expected);
      });

      test("should return new state with next block", () => {
        let stateBefore = {
          block: testBlock,
          blockPosition: { x: 1, y: 3 },
          grid: testGrid1
        };

        let expected = {
          block: testBlock,
          blockPosition: { x: 0, y: 0 },
          grid: testGrid3
        };

        expect(tick(stateBefore)) |> toEqual(expected);
      });

    });
  });
