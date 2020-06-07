open Jest;
open Expect;
open ReactTestingLibrary;

// Auto cleanup after each test was intentionally disabled
// Actually this is a one big test with set of sequental actions
// Tests like this probably should be written on JS since multiple expectations are convinient here

beforeAll(() => Jest.useFakeTimers());

describe("Game component actions", () => {
  let rendered = render(<Game />);

  let container = rendered |> container;

  test("Render", () => {
    expect(container) |> toMatchSnapshot
  });

  let startButton =
    rendered |> getByText(~matcher=`Str("Start Game"), ~options=?None);

  test("Start button", () => {
    expect(startButton) |> toMatchSnapshot
  });

  test("Click Start", () => {
    act(() => startButton |> FireEvent.click(~eventInit=?None));

    let numberThree =
      rendered |> getByText(~matcher=`Str("3"), ~options=?None);

    expect(numberThree) |> toMatchSnapshot;
  });

  test("After 1 second", () => {
    act(() => Jest.advanceTimersByTime(1000));

    let numberTwo =
      rendered |> getByText(~matcher=`Str("2"), ~options=?None);

    expect(numberTwo) |> toMatchSnapshot;
  });

  test("After 2 seconds", () => {
    act(() => Jest.advanceTimersByTime(1000));
    let three = rendered |> getByText(~matcher=`Str("1"), ~options=?None);
    expect(three) |> toMatchSnapshot;
  });

  test("Game started", () => {
    act(() => Jest.advanceTimersByTime(1000));

    let scoreText =
      rendered |> getByText(~matcher=`Str("Score"), ~options=?None);

    expect(scoreText) |> toMatchSnapshot;
  });
});
