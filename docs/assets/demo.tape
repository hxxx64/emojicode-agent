# VHS tape for emojicode agent demo
# To run: vhs docs/assets/demo.tape

# Output file
Output docs/assets/demo.gif

# Terminal settings
Set Shell zsh
Set FontSize 16
Set Width 1200
Set Height 600
Set Padding 30
Set WindowBar Colorful
Set TypingSpeed 50ms

# Build and start the agent
Hide
Type "make"
Enter
Sleep 2.5s
Show

# Wait for the agent's ascii banner and provider info
Sleep 1.5s

# Ask the agent to add a new class
Type "add a Bird class, similar to existing Cat class"
Sleep 500ms
Enter

# The agent will now process. 
# It typically takes:
# 1. Thinking...
# 2. Reading cat.js
# 3. Thinking...
# 4. Writing bird.js
# 5. Final response
Sleep 16s
