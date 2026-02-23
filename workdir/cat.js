class Cat {
  constructor(name) {
    this.name = name;
  }

  meow() {
    console.log(`${this.name} says: Meow!`);
  }

  sleep() {
    console.log(`${this.name} is sleeping...`);
  }

  sayHello() {
    console.log(`${this.name} says: Hello!`);
  }
}

module.exports = Cat;
