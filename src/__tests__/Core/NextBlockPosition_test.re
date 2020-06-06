open Jest;
open Expect;

open Types;
open Types.Direction;

let pos = {x: 2, y: 2};

test("Move left", () => {
  expect(Core.nextBlockPosition(Left, pos))
  |> toEqual({x: pos.x - 1, y: pos.y})
});

test("Move right", () => {
  expect(Core.nextBlockPosition(Right, pos))
  |> toEqual({x: pos.x + 1, y: pos.y})
});
