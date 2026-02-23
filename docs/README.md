# 🍇 emojicode agent: Documentation

**emojicode agent** is an autonomous AI coding assistant built entirely in [Emojicode](https://www.emojicode.org/).

---

### 🚀 Core Capabilities

The agent operates through a suite of specialized tools designed for precise codebase manipulation. All interactions use an XML-based protocol.

| Tool                  | XML Structure                                                         | Purpose / Description                            | Key Feature                |
| :-------------------- | :-------------------------------------------------------------------- | :----------------------------------------------- | :------------------------- |
| **`read_file`**       | `<read_file><path>...</path></read_file>`                             | Reads the full content of a file.                | Path-traversal protection  |
| **`write_file`**      | `<write_file><path>...</path><content>...</content></write_file>`     | Creates or overwrites a file.                    | Atomic updates             |
| **`replace_in_file`** | `<replace_in_file><path>...</path><diff>...</diff></replace_in_file>` | Targeted search-and-replace using a diff format. | Context-aware edits        |
| **`search_files`**    | `<search_files><path>...</path><search>...</search></search_files>`   | Recursively searches for a string.               | Recursive pattern matching |
| **`list_files`**      | `<list_files><path>...</path><recursive>...</recursive></list_files>` | Lists files in a directory.                      | Full tree awareness        |

---

### 🔄 The Agentic Loop: "Think-Act-Observe"

The agent doesn't just execute commands; it navigates a self-correcting cognitive loop (up to 10 turns per task):

1.  **Analyze**: Evaluates the user's intent against the session history.
2.  **Act**: Invokes tools to gather data or apply changes.
3.  **Observe**: Captures tool output (success or failure) as new context.
4.  **Refine**: Adjusts its strategy based on observations until the goal is met.

> [!NOTE]
> All interactions are **stateful**. The agent maintains a complete history of messages, tool calls, and results within a `session` entity.

---

### 🏗️ Architectural Blueprint

The project adheres to **Hexagonal Architecture** (Ports & Adapters), ensuring the core logic remains decoupled from infrastructure.

#### **Layered Design**
*   **Domain**: Pure Emojicode. Defines entities (`session`), value objects (`message`), and interfaces (`ports`).
*   **Application**: Orchestrates the agent's "brain"—the tool executor and loop logic.
*   **Infrastructure**: The "hands." Concrete adapters for OpenRouter, Terminal I/O, and the C++ FFI bridge.

#### **Project Layout**
```text
src/
├── domain/            # Pure Business Logic (No dependencies)
│   ├── entities/      # Objects with identity (session.entity.🍇)
│   ├── value-objects/ # Immutable data (message.vo.🍇, tool-call.vo.🍇, tool-result.vo.🍇)
│   ├── services/      # Domain logic (prompt.service.🍇, tool-parser.service.🍇)
│   └── ports/         # Protocols/Interfaces (ai.port.🍇, file-system.port.🍇, input.port.🍇, output.port.🍇)
│
├── application/       # Use Cases & Orchestration
│   └── services/      # Orchestrators (agent.service.🍇, tool-executor.service.🍇)
│
├── infrastructure/    # External Implementation Details
│   ├── adapters/      # Concrete implementations (ai/, persistence/, ui/)
│   │   └── *.cpp      # C++ FFI for HTTP, File I/O, and Input
│   └── config/        # Infrastructure-specific config (system-prompt.config.🍇)
│
├── tests/             # Unit and integration tests (main.🍇)
│
└── main.🍇            # Composition Root (Dependency Injection)
```


#### **C++ FFI Mechanism**
Since Emojicode is a high-level language, it leverages a C++ Foreign Function Interface (FFI) for low-level system operations.
- **Protocols**: Defined in `.🍇` files using the `external` keyword.
- **Implementation**: Provided in corresponding `.cpp` files.
- **Bridge**: The `Makefile` links the Emojicode object files with the compiled C++ code.

Example FFI mapping:
- `src/domain/ports/file-system.port.🍇` defines the protocol.
- `src/infrastructure/adapters/persistence/native-file-system.adapter.cpp` implements it.

---

### 🧪 Testing

The project maintains a rigorous testing suite using the **testtube** library, covering both isolated domain logic and end-to-end agentic loops.

#### **Test Suite Structure**
-   **`src/tests/unit-tests/`**: Validates pure domain logic (e.g., `session`, `tool-parser`) without side effects.
-   **`src/tests/integration-tests/`**: Tests the interaction between services, typically using mocks for the AI and File System adapters.
-   **`src/tests/utils/`**: Contains shared test helpers and mock implementations (e.g., `mock-ai.adapter.🍇`).
-   **`src/tests/main.🍇`**: The global test runner that orchestrates all suites and reports failures.

---

### 🛡️ Safety & Security by Design

*   **Sandboxing**: Operations are restricted to a dedicated `workdir/`. Absolute paths and directory traversal (`..`) are strictly prohibited in the `tool-executor` layer.
*   **Hardened I/O**: File and network operations are routed through a C++ FFI layer, providing fine-grained control over system calls.
*   **Input Validation**: All tool parameters are rigorously parsed and validated before execution.

---

### 🔌 Extensibility

The agent is built for growth.

#### **Adding a New AI Provider**
1. Define a new adapter in `src/infrastructure/adapters/ai/`.
2. Implement the `ai.port.🍇` protocol.
3. Register the new adapter in `src/main.🍇`.

#### **Adding a New Tool**
1. **Domain**: Update `src/domain/services/tool-parser.service.🍇` to recognize the new XML tags.
2. **Application**: Add a handler in `src/application/services/tool-executor.service.🍇` to map the tool to a file system operation.
3. **Infrastructure**: If needed, add a new method to `src/domain/ports/file-system.port.🍇` and implement it in C++.
4. **Prompt**: Update `src/infrastructure/config/system-prompt.config.🍇` so the AI knows how to use the tool.