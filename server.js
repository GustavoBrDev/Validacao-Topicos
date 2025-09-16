// server.js
const express = require('express');
const http = require('http');
const cors = require('cors');
const mqtt = require('mqtt');
const { Server } = require('socket.io');

const MQTT_BROKER = 'mqtt://broker.hivemq.com:1883'; 
const BASE_TOPIC = 'ifsc/ctds/topicos/validacao/gustavo-stinghen';

const app = express();
app.use(cors());
app.use(express.json());

const server = http.createServer(app);
const io = new Server(server, { cors: { origin: '*' } });

const mqttClient = mqtt.connect(MQTT_BROKER);

mqttClient.on('connect', () => {
  mqttClient.subscribe(`${BASE_TOPIC}/#`, { qos: 0 }, (err) => {
    if (err) console.warn('Erro subscribe', err);
  });
});

mqttClient.on('error', (err) => console.error('MQTT error', err));
mqttClient.on('close', () => console.log('MQTT desconectado'));

mqttClient.on('message', (topic, payload) => {
  const msg = payload.toString();
  io.emit('mqtt_message', { topic, payload: msg });
});

app.post('/publish', (req, res) => {
  const { topic, payload } = req.body;
  if (!topic || payload === undefined) return res.status(400).json({ error: 'topic e payload required' });
  mqttClient.publish(topic, String(payload), { qos: 0, retain: false }, (err) => {
    if (err) return res.status(500).json({ ok: false, err: err.message });
    res.json({ ok: true });
  });
});

io.on('connection', (socket) => {
  console.log('Client WS conectado', socket.id);

  socket.on('publish', ({ topic, payload }) => {
    if (!topic || payload === undefined) return;
    mqttClient.publish(topic, String(payload));
  });

  socket.on('disconnect', () => {
    console.log('Client WS desconectou', socket.id);
  });
});

const PORT = 3000;
server.listen(PORT, () => console.log(`Proxy rodando em http://localhost:${PORT}`));
