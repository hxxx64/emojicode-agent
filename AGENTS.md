# 🤖 Agent Mission Briefing

This project is an AI coding agent built in **Emojicode**. Your mission is to maintain and extend its capabilities while strictly adhering to its architectural principles and language conventions.

---

### 📖 Essential References

Before contributing, you **must** review the following documentation:

1.  📄 **[Documentation](./docs/README.md)**: **Primary reference** for architecture (Hexagonal/Ports & Adapters), project layout, core capabilities, and the agentic tool loop.
2.  📄 **[Official Emojicode Documentation](https://www.emojicode.org/docs/)**: Authoritative reference for syntax, types, and standard library.
3.  📄 **[README.md](./README.md)**: High-level overview, quick start, and system requirements.

---

### 🍇 Emojicode Quick-Start for AI

-   **File Extensions**: Use `.🍇` (preferred) or `.emojic`.
-   **Entry Point**: `🏁 🍇 ... 🍉`
-   **Type System**: Strongly typed using emojis (e.g., `🔡` String, `🔢` Integer, `👌` Boolean).
-   **Structure**: Follow the existing indentation and emoji spacing found in `src/main.🍇`.
-   **C++ FFI**: Native logic resides in `.cpp` files located adjacent to their `.🍇` adapter declarations in `src/infrastructure/`.

---

### ⚡ Developer Workflow

-   **Build & Run**: `make` (compiles and launches the agent in `workdir/`).
-   **Test**: `make test` (executes the full Emojicode test suite).
-   **Environment**: Configure your AI provider keys in a `.env` file (see `.env.example`).

---

### 🎯 Contribution Rules

1.  **Strict Architecture**: Respect the boundaries between Domain, Application, and Infrastructure layers as detailed in the **[Documentation](./docs/README.md)**.
2.  **Type Integrity**: Emojicode is strictly typed. Always verify types in the `src/domain/ports/` or the official language reference.
3.  **Surgical Edits**: Prioritize precise logic updates in Emojicode. Use the C++ FFI layer only for performance-critical or system-level operations.
4.  **Sync Docs**: Update `docs/README.md` if you introduce new tools, providers, or significant architectural changes.
