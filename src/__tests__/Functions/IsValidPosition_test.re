open Jest;
open Expect;
open Types;
open Core

let block = [|
  [|X, X|],
  [|X, X|]
|];

let grid = [|
  [|O, O, O, O|],
  [|O, O, O, O|],
  [|O, O, O, O|],
  [|O, O, O, O|],
|];

describe("Is valid position?", () => {
  test("Valid", () => {
    let result = isValidPosition({x: 2, y: 2}, block, grid);
    expect(result) |> toEqual(true);
  });

  test("X Not valid", () => {
    let result = isValidPosition({x: (-1), y: 0}, block, grid);
    expect(result) |> toEqual(false);
  });

  test("X Not valid", () => {
    let result = isValidPosition({x: 3, y: 0}, block, grid);
    expect(result) |> toEqual(false);
  });

  test("Y Not valid", () => {
    let result = isValidPosition({x: 0, y: 3}, block, grid);
    expect(result) |> toEqual(false);
  });
});
