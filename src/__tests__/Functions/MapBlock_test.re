open Jest;
open Expect;
open Types;
open Functions;

describe("map block row to grid row", () => {
  let blockRow = [|X, O, X, O|];
  let gridRow = [|O, X, O, O, O, O|];

  test("map row", () => {
    let result = mapBlockRowToGridRow(~posX=0, ~blockRow, ~gridRow);
    let expected = [|X, X, X, O, O, O|];

    expect(result) |> toEqual(expected);
  });
});

describe("map block to grid", () => {
  let block = Blocks.blockT;

  let grid = [|
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|X, O, X, O, O, O|],
  |];

  test("map block", () => {
    let result = mapBlockToGrid({
                   blockPosition: {
                     x: 0,
                     y: 2,
                   },
                   block,
                   grid,
                 });

    let expected = [|
      [|O, O, O, O, O, O|],
      [|O, O, O, O, O, O|],
      [|X, X, X, O, O, O|],
      [|X, X, X, O, O, O|],
    |];

    expect(result) |> toEqual(expected);
  });
});
