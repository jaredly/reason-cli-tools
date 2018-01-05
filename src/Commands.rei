
type job;
 /* = (unit => unit, unit => unit); */

/**
 * Get the output of a command, in lines, and whether it succeeded.
 */
let execSync: (~cmd: string, ~onOut: string => unit=?, unit) => (list(string), bool);

let exec: (~cmd: string, ~onOut: string => unit) => job;

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
  ) => job;

let kill: job => unit;
let poll: job => unit;

/**
 * Run an async command until it exits.
 */
let run: job => unit;

/**
 * Run multiple async commands in parallel until they all exit.
 */
let runAll: list(job) => unit;