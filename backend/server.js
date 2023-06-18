const express = require('express');
const WebSocket = require('ws');

const app = express();

// Create an HTTP server using the Express app
const server = app.listen(8080, () => {
  console.log('WebSocket server is running on port 8080');
});

// Create a WebSocket server
const wss = new WebSocket.Server({ server });

// Event handler for when a client connects to the server
wss.on('connection', (ws) => {
  console.log('A client connected');

  // Event handler for messages received from the client
  ws.on('message', (message) => {
    console.log('Received message:', message.toString());
    ws.send(message.toString());
    // Echo the message back to all connected clients
    wss.clients.forEach((client) => {
      if (client !== ws && client.readyState === WebSocket.OPEN) {
        client.send(message.toString());
      }
    });
  });

  // Event handler for when the client disconnects
  ws.on('close', () => {
    console.log('A client disconnected');
  });
});
