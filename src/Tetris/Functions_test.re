open Jest;
open Expect;
/* open Blocks; */
open Functions;

let testBlock = [|
[|X, X, X|],
[|O, X, O|],
|];

let testGrid =[|
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|O, O, O, O, O, O|],
[|X, O, X, O, O, O|],
|];

let testGridWithStrokes =[|
[|O, O, O, O, O, O|],
[|O, O, X, O, O, O|],
[|O, X, O, O, O, O|],
[|X, X, X, X, X, X|],
[|O, X, O, X, O, O|],
[|X, X, X, X, X, X|],
|];


let () =
  describe("Tetris basic functions tests", () => {
    describe("canMapRow", () => {
      test("Should return true", () => {
        let blockRow = [|X, X, X|];
        let gridRow = [|O, O, O, O, O, O, O, O, O, O|];
        let result = canMapRow(0, blockRow, gridRow);

        expect(result) |> toEqual(true);
      });

      test("Should return true", () => {
        let blockRow = [|X, O, X|];
        let gridRow = [|O, X, O, O, O, O, O, O, O, O|];
        let result = canMapRow(0, blockRow, gridRow);

        expect(result) |> toEqual(true);
      });

      test("Should return true", () => {
        let blockRow = [|O, O, O|];
        let gridRow = [|O, X, O, O, O, O, O, O, O, O|];
        let result = canMapRow(1, blockRow, gridRow);

        expect(result) |> toEqual(true);
      });

      test("Should return false", () => {
        let blockRow = [|X, O, X|];
        let gridRow = [|O, X, O, O, O, O, O, O, O, O|];
        let result = canMapRow(1, blockRow, gridRow);

        expect(result) |> toEqual(false);
      });
    });

    describe("canMapBlock", () => {
      test("Should return true", () => {
        let result = canMapBlock(0, 4, testBlock, testGrid);

        expect(result) |> toEqual(true);
      });

      test("Should return false", () => {
        let result = canMapBlock(1, 4, testBlock, testGrid);

        expect(result) |> toEqual(false);
      });
    });

    describe("map block row to grid row", () => {
      test("map row imperative", () => {
        let blockRow = [|X, X, X|];
        let gridRow = [|O, O, O, O, O, O, O, O, O, O|];

        let result = mapBlockRowToGridRowImp(0, blockRow, gridRow);
        let expected = [|X, X, X, O, O, O, O, O, O, O|];

        expect(result) |> toEqual(expected);
      });

      test("map row functional", () => {
        let blockRow = [|X, X, X|];
        let gridRow = [|O, O, O, O, O, O, O, O, O, O|];

        let result = mapBlockRowToGridRow(0, blockRow, gridRow);
        let expected = [|X, X, X, O, O, O, O, O, O, O|];

        expect(result) |> toEqual(expected);
      });
    });

    describe("map block to grid", () => {
      test("map block imperative", () => {

        let result = mapBlockToGridImp(0, 4, testBlock, testGrid);
        let expected =[|
        [|O, O, O, O, O, O|],
        [|O, O, O, O, O, O|],
        [|O, O, O, O, O, O|],
        [|O, O, O, O, O, O|],
        [|X, X, X, O, O, O|],
        [|X, X, X, O, O, O|],
        |];

        expect(result) |> toEqual(expected);
      });
    });

    describe("get stroke indexes", () => {
      test("must be 2", () => {

        let result = getStrokeIndexes(testGridWithStrokes);
        let expected =[|5, 3|];

        expect(result) |> toEqual(expected);
      });

      test("must return empty array", () => {

        let result = getStrokeIndexes(testGrid);
        let expected =[||];

        expect(result) |> toEqual(expected);
      });
    });
  });
