open Jest;
open Expect;

test("Score is 499, level 1", () => {
  expect(Core.calcLevel(499)) |> toEqual(1)
});

test("Score is 999, level 2", () => {
  expect(Core.calcLevel(999)) |> toEqual(2)
});

test("Score is 1000, level 3", () => {
  expect(Core.calcLevel(1000)) |> toEqual(3)
});
