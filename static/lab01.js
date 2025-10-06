function updateBatteryStatus() {
    fetch('/api/lab01')
        .then(response => response.json())
        .then(data => {
            document.getElementById('charge').textContent = data.charge;
            document.getElementById('powerMode').textContent = data.powerMode;
            document.getElementById('status').textContent = data.status;

            let timeLeft = data.timeLeft;
            if (timeLeft === -1) {
                document.getElementById('timeLeft').textContent = "Unknown";
            } else {
                let hours = Math.floor(timeLeft / 3600);
                let minutes = Math.floor((timeLeft % 3600) / 60);
                let seconds = timeLeft % 60;
                document.getElementById('timeLeft').textContent =
                    `${hours}h ${minutes}m ${seconds}s`;
            }

            document.getElementById('eco').textContent = data.eco;
            document.getElementById('batteryInfo').textContent = data.batteryInfo;

            if (data.powerMode === 'Online') {
                document.getElementById('chargingGif').style.display = 'block';
            } else {
                document.getElementById('chargingGif').style.display = 'none';
            }
        })
        .catch(error => console.error('Error fetching battery status:', error));
}

function sleep() {
    fetch('/api/sleep')
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                alert('Going to sleep...');
            } else {
                alert('Failed to go to sleep.');
            }
        })
        .catch(error => console.error('Error going to sleep:', error));
}

function hibernate() {
    fetch('/api/hibernate')
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                alert('Going to hibernate...');
            } else {
                alert('Failed to hibernate.');
            }
        })
        .catch(error => console.error('Error hibernating:', error));
}

setInterval(updateBatteryStatus, 5000);
window.onload = updateBatteryStatus;