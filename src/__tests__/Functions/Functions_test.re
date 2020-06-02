open Jest;
open Expect;
open Types;
open Core

let testBlock = [|[|X, X, X|], [|O, X, O|]|];

let testGrid = [|
  [|O, O, O, O, O, O|],
  [|O, O, O, O, O, O|],
  [|O, O, O, O, O, O|],
  [|O, O, O, O, O, O|],
  [|O, O, O, O, O, O|],
  [|X, O, X, O, O, O|],
|];

let testGridWithStrokes = [|
  [|O, O, O, O, O, O|],
  [|O, O, X, O, O, O|],
  [|O, X, O, O, O, O|],
  [|X, X, X, X, X, X|],
  [|O, X, O, X, O, O|],
  [|X, X, X, X, X, X|],
|];

describe("Tetris basic functions tests", () => {
  describe("get stroke indexes", () => {
    test("must be 2", () => {
      let result = getStrokeIndexes(testGridWithStrokes);
      let expected = [|5, 3|];

      expect(result) |> toEqual(expected);
    });

    test("must return empty array", () => {
      let result = getStrokeIndexes(testGrid);
      let expected = [||];

      expect(result) |> toEqual(expected);
    });
  });

  describe("removeFilledRows", () => {
    test("must be ok (nothing to remove)", () => {
      let grid = [|
        [|O, O, O|],
        [|X, O, X|],
        [|X, O, X|],
        [|X, X, O|],
        [|X, O, X|],
      |];

      let expected = [|
        [|O, O, O|],
        [|X, O, X|],
        [|X, O, X|],
        [|X, X, O|],
        [|X, O, X|],
      |];

      let indexesToRemove = getStrokeIndexes(grid);

      let result = removeFilledRows(grid, indexesToRemove);

      expect(result) |> toEqual(expected);
    });

    test("must be ok", () => {
      let grid = [|
        [|O, O, O|],
        [|X, O, X|],
        [|X, X, X|],
        [|O, X, O|],
        [|X, X, X|],
      |];

      let expected = [|
        [|O, O, O|],
        [|O, O, O|],
        [|O, O, O|],
        [|X, O, X|],
        [|O, X, O|],
      |];

      let indexesToRemove = getStrokeIndexes(grid);

      let result = removeFilledRows(grid, indexesToRemove);

      expect(result) |> toEqual(expected);
    });

    test("must be ok", () => {
      let grid = [|
        [|O, O, O|],
        [|X, O, X|],
        [|X, X, X|],
        [|X, X, X|],
        [|X, X, X|],
      |];

      let expected = [|
        [|O, O, O|],
        [|O, O, O|],
        [|O, O, O|],
        [|O, O, O|],
        [|X, O, X|],
      |];

      let indexesToRemove = getStrokeIndexes(grid);

      let result = removeFilledRows(grid, indexesToRemove);

      expect(result) |> toEqual(expected);
    });

    test("must be ok", () => {
      let grid = [|
        [|X, O, X|],
        [|X, X, X|],
        [|X, X, X|],
        [|X, X, X|],
        [|X, X, X|],
        [|X, O, X|],
      |];

      let expected = [|
        [|O, O, O|],
        [|O, O, O|],
        [|O, O, O|],
        [|O, O, O|],
        [|X, O, X|],
        [|X, O, X|],
      |];

      let indexesToRemove = getStrokeIndexes(grid);

      let result = removeFilledRows(grid, indexesToRemove);

      expect(result) |> toEqual(expected);
    });
  });

  describe("tick", () => {
    test("must be 2", () => {
      let result = getStrokeIndexes(testGridWithStrokes);
      let expected = [|5, 3|];

      expect(result) |> toEqual(expected);
    });

    test("must return empty array", () => {
      let result = getStrokeIndexes(testGrid);
      let expected = [||];

      expect(result) |> toEqual(expected);
    });
  });
});

test("calcLevel", () =>
  expect(calcLevel(5000)) |> toEqual(11)
);

// For test coverage
// Compiler optimizes code and removes this abstractions so functions below not called
test("getWidth", () => {
  let result = getWidth([|[|O, O|]|]);
  expect(result) |> toEqual(2);
});

test("getHeight", () => {
  let result = getHeight([|[|O, O|]|]);
  expect(result) |> toEqual(1);
});
