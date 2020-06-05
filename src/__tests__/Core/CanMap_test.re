open Jest;
open Expect;
open Types.Cell;
open Core

describe("Can map row?", () => {
  let blockRow = [|X, O, X, O|];
  let gridRow = [|O, X, O, O, O, O|];

  test("Should return true", () => {
    let result = canMapRow(0, blockRow, gridRow);

    expect(result) |> toEqual(true);
  });

  test("Should return true", () => {
    let result = canMapRow(2, blockRow, gridRow);

    expect(result) |> toEqual(true);
  });

  test("Should return false", () => {
    let result = canMapRow(1, blockRow, gridRow);

    expect(result) |> toEqual(false);
  });
});

describe("Can map block?", () => {
  let block = [|
    [|X, X, X|],
    [|O, X, O|],
  |];

  let grid = [|
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|O, O, O, X, X, X|],
    [|X, O, X, X, X, X|],
  |];

  test("Can map", () => {
    let result = canMapBlock({x: 0, y: 2}, block, grid);

    expect(result) |> toEqual(true);
  });

  test("Can't map", () => {
    let result = canMapBlock({x: 2, y: 2}, block, grid);

    expect(result) |> toEqual(false);
  });

  test("Can't map", () => {
    let result = canMapBlock({x: 4, y: 0}, block, grid);

    expect(result) |> toEqual(false);
  });
});
