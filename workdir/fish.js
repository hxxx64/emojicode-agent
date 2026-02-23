// fish.js
class Fish {
  constructor(name) {
    this.name = name;
  }

  swim() {
    console.log(`${this.name} is swimming.`);
  }

  eat() {
    console.log(`${this.name} is eating...`);
  }
}

module.exports = Fish;
