open Jest;
open Expect;
open Types.Screen;

test("Title -> Counter", () => {
  expect(Core.nextScreen(Title)) |> toEqual(Counter)
});

test("Counter -> Game", () => {
  expect(Core.nextScreen(Counter)) |> toEqual(Game)
});

test("Game -> Gameover", () => {
  expect(Core.nextScreen(Game)) |> toEqual(Gameover)
});

test("Gameover -> Counter", () => {
  expect(Core.nextScreen(Game)) |> toEqual(Gameover)
});
