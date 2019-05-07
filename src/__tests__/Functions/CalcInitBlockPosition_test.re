open Jest;
open Expect;
open Types;
open Functions;

test("Block appears in a middle of a grid", () => {
  let block = Blocks.blockBox;
  let result = calcInitBlockPosition(~block, ~gridWidth=8);

  expect(result) |> toEqual({x: 3, y: 1});
});
