const mqtt = require('mqtt');

// Connect to MQTT broker
const client = mqtt.connect('mqtt://localhost:1883'); // Replace with your MQTT broker address

client.on('connect', () => {
    console.log('Connected to MQTT broker');
    
    // Publish sensor data every second
    setInterval(() => {
        const temperature = (Math.random() * (35 - 15) + 15).toFixed(2); // 15°C - 35°C
        const humidity = (Math.random() * (90 - 30) + 30).toFixed(2); // 30% - 90%
        
        const payload = JSON.stringify({ temperature, humidity });
        client.publish('sensor/data', payload); // Publish to topic 'sensor/data'
        
        console.log(`Published: ${payload}`);
    }, 1000);
});

client.on('error', (error) => {
    console.error('Error connecting to MQTT broker:', error);
});
