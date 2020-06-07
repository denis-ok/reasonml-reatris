open Jest;
open Expect;
open ReactTestingLibrary;
open Belt;

let getByString = text => getByText(~matcher=`Str(text), ~options=?None);

// Auto cleanup after each test was intentionally disabled
// Actually this is a one big test with set of sequental actions
// Tests like this probably should be written on JS since multiple expectations are convinient here

beforeAll(() => Jest.useFakeTimers());

describe("Game component", () => {
  let rendered = render(<Game />);

  let container = rendered |> container;

  test("Render", () => {
    expect(container) |> toMatchSnapshot
  });

  let startButton = rendered |> getByString("Start Game");

  test("Render Start button", () => {
    expect(startButton) |> toMatchSnapshot
  });

  test("Number 3 Displayed after click Start", () => {
    act(() => startButton |> FireEvent.click(~eventInit=?None));

    let numberThree = rendered |> getByString("3");

    expect(numberThree) |> toMatchSnapshot;
  });

  test("Number 2 Displayed after 1 second", () => {
    act(() => Jest.advanceTimersByTime(1000));

    let numberTwo = rendered |> getByString("2");

    expect(numberTwo) |> toMatchSnapshot;
  });

  test("Number 1 Displayed after 2 seconds", () => {
    act(() => Jest.advanceTimersByTime(1000));

    let numberThree = rendered |> getByString("1");

    expect(numberThree) |> toMatchSnapshot;
  });

  test("Game started, Score displayed", () => {
    act(() => Jest.advanceTimersByTime(1000));

    let scoreText = rendered |> getByString("Score");

    expect(scoreText) |> toMatchSnapshot;
  });

  test("Render Play Again button", () => {
    act(() => Array.make(150, 500)->Array.forEach(Jest.advanceTimersByTime));

    let gameOverText = rendered |> getByString("Play Again?");
    expect(gameOverText) |> toMatchSnapshot;
  });

  test("Click on 'Play Again' shows countdown counter", () => {
    let playAgainButton = rendered |> getByString("Play Again?");
    act(() => playAgainButton |> FireEvent.click(~eventInit=?None));

    let numberThree = rendered |> getByString("3");
    expect(numberThree) |> toMatchSnapshot;
  });
});
