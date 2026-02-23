# workdir

This is the default working directory the agent operates on.
It contains a small Node.js codebase — simple enough to be fully readable, varied enough to exercise all agent tools.

Point the agent at any real project instead:

```bash
make WORKDIR=../my-project
```

---

## Files

| File           | Description                                            |
| -------------- | ------------------------------------------------------ |
| `index.js`     | Entry point — imports all animals and runs a demo      |
| `cat.js`       | `Cat` class with `meow()`, `sleep()`, and `sayHello()` |
| `dog.js`       | `Dog` class with `bark()`                              |
| `fish.js`      | `Fish` class with `swim()` and `eat()`                 |
| `package.json` | Minimal package manifest                               |

---

## Example prompts

```
add a Bird class, similar to the other animals
```
```
cat.js has a bug — find and fix it
```
```
list all files and summarize what each one does
```
```
add a speak() method to every animal class
```
