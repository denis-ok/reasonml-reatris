open Jest;
open Expect;
open Types;
open Functions;

let block = [|[|X, X, X|], [|O, X, O|]|];

let nextBlock = Blocks.getRandomBlock();

test("Simple tick: increment block position and score by 1", () => {
  let grid = [|
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|X, O, X, X, X, X|],
  |];

  let stats = {score: 10, lines: 0, level: 1};

  let gridState = {
    block,
    blockPosition: {
      x: 0,
      y: 3,
    },
    grid,
  };

  let result = tick(gridState, stats, ~nextBlock, ());

  expect(result)
  |> toEqual({
       gridState: {
         block,
         blockPosition: {
           x: 0,
           y: 4,
         },
         grid,
       },
       stats: {
         score: 11,
         lines: 0,
         level: 1,
       },
       gameOver: false,
       nextBlockToShow: nextBlock,
     });
});

describe("Block placed", () => {
  let grid = [|
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|X, O, X, X, X, X|],
  |];

  let gridNew = [|
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|O, O, X, X, X, O|],
    [|O, O, O, X, O, O|],
    [|X, O, X, X, X, X|],
  |];

  let stats = {score: 10, lines: 0, level: 1};

  let gridState = {
    block,
    blockPosition: {
      x: 2,
      y: 3,
    },
    grid,
  };

  let result = tick(gridState, stats, ~nextBlock, ());

  test("Grid updated", () =>
    expect(result.gridState.grid) |> toEqual(gridNew)
  );

  test("Next block used", () =>
    expect(result.gridState.block) |> toEqual(nextBlock)
  );

  test("Game continues", () =>
    expect(result.gameOver) |> toEqual(false)
  );
});

describe("Block placed, rows cleared, stats with level updated", () => {
  let grid = [|
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|X, O, X, X, X, X|],
  |];

  let gridNew = [|
    [|O, O, O, O, O, O|],
    [|O, O, O, O, O, O|],
    [|X, X, X, O, O, O|],
  |];

  let stats = {score: 450, lines: 0, level: 1};

  let gridState = {
    block,
    blockPosition: {
      x: 0,
      y: 1,
    },
    grid,
  };

  let result = tick(gridState, stats, ~nextBlock, ());

  test("Grid updated", () =>
    expect(result.gridState.grid) |> toEqual(gridNew)
  );

  test("Next block used", () =>
    expect(result.gridState.block) |> toEqual(nextBlock)
  );

  test("Score updated", () =>
    expect(result.stats) |> toEqual({score: 501, lines: 1, level: 2})
  );
});

describe("Game over", () => {
  let grid = [|
    [|X, X, X, X, X, O|],
    [|X, X, X, X, O, X|],
    [|X, X, X, O, X, X|],
    [|X, X, O, X, X, X|],
  |];

  let stats = {score: 450, lines: 0, level: 1};

  let gridState = {
    block,
    blockPosition: {
      x: 2,
      y: 2,
    },
    grid,
  };

  let result = tick(gridState, stats, ~nextBlock=block, ());

  test("Game over", () =>
    expect(result.gameOver) |> toEqual(true)
  );
});
