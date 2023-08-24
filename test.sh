#!/usr/bin/env bash

# To be run from the project directory, once the code is built
rm -r ./.versions

# Create a new commit
cat > "test.txt" << EOF
0
EOF

./build/vcs create test.txt "Zero"

# Create a new commit
cat > "test.txt" << EOF
0
1
EOF

./build/vcs create test.txt "One"

# Create a new branch and switch to it
./build/vcs branch "new_branch"
./build/vcs switch "new_branch"

# Create a new commit
cat > "test.txt" << EOF
0
1
2
EOF

./build/vcs create test.txt "Two"