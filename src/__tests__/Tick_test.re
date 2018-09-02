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

let testGrid4 =[|
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
|];

let testGrid4After =[|
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|X, X, X, O, O, O|],
[|O, X, O, O, O, O|],
|];


let () =
  describe("Tetris basic functions tests", () => {
    describe("tick", () => {

      test("Must update coord only", () => {
        let statsBefore = {
          score: 10,
          lines: 0,
          level: 1
        };

        let gridStateBefore = {
          block: testBlock,
          blockPosition: { x: 0, y: 3 },
          grid: testGrid1
        };

        let statsAfter = {
          score: 11,
          lines: 0,
          level: 1
        };

        let gridStateAfter = {
          block: testBlock,
          blockPosition: { x: 0, y: 4 },
          grid: testGrid1
        };

        let expected = {
          gridState: gridStateAfter,
          stats: statsAfter,
          gameOver: false,
          nextBlockToShow: testBlock
        };

        expect(tick(gridStateBefore, statsBefore, ~nextBlock=testBlock, ())) |> toEqual(expected);
      });

      /* test("should return new state after strike", () => {
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
      }); */

      /* test("should return new state with next block 1", () => {
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
      }); */

      /* test("should return new state with next block 2", () => {
        let stateBefore = {
          block: testBlock,
          blockPosition: { x: 0, y: 4 },
          grid: testGrid4
        };

        let expected = {
          block: testBlock,
          blockPosition: { x: 0, y: 0 },
          grid: testGrid4After
        };

        expect(tick(stateBefore)) |> toEqual(expected);
      }); */

    });
  });
