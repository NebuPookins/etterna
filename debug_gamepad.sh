#!/bin/bash

# Debug script for gamepad input issues
# This script runs Etterna with enhanced logging and monitors the logs

echo "Starting Etterna with gamepad debugging enabled..."

# Create logs directory if it doesn't exist
mkdir -p Logs

# Run Etterna with debug logging
# The logs will be written to Logs/ and also to console
./Etterna --verbose --log-level=trace 2>&1 | tee Logs/gamepad_debug_$(date +%Y%m%d_%H%M%S).log &

# Get the PID of the Etterna process
ETTERNA_PID=$!

echo "Etterna started with PID: $ETTERNA_PID"
echo "Logs are being written to Logs/gamepad_debug_*.log"
echo ""
echo "To monitor the logs in real-time, run:"
echo "tail -f Logs/gamepad_debug_*.log | grep -E '(LinuxJoystick|Player::Step|ScreenGameplay|SetJudgment)'"
echo ""
echo "To stop the game, press Ctrl+C or run: kill $ETTERNA_PID"
echo ""

# Wait for the game to finish
wait $ETTERNA_PID

echo "Etterna has exited." 