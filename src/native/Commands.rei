/**
 * Get the output of a command, in lines, and whether it succeeded.
 */
let readCommand: (~cmd: string, ~onOut: string => unit=?, unit) => (list(string), bool);

/**
 * Returns a poll function, and a close function. Useful for running
 * multiple commands in parallel, without needing any special async libraries.
 */
let pollableCommand: (~cmd: string, ~onOut: string => unit) => (unit => unit, unit => unit);

/**
 * Returns a poll function, and a close function. Checks every `checkInterval`
 * to see if the command has died, and if so it restarts it, calling `onStart`.
 * It also calls `onStart` initially.
 *
 * `checkInterval` defaults to 1.0 (one second).
 */
let keepAlive:
  (
    ~cmd: string,
    ~onOut: string => unit=?,
    ~onErr: string => unit=?,
    ~onStart: unit => unit=?,
    ~checkInterval: float=?,
    unit
  ) =>
  (unit => unit, unit => unit);