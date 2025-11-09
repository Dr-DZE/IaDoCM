document.addEventListener('DOMContentLoaded', () => {
    const logsContainer = document.getElementById('logs-container');
    const devicesList = document.getElementById('devices-list');
    const disableMouseButton = document.getElementById('disable-mouse-btn');
    const ejectAllButton = document.getElementById('eject-all-btn');

    // Fetch and display logs
    const fetchLogs = async () => {
        try {
            const response = await axios.get('/api/lab05/logs');
            logsContainer.textContent = response.data;
            logsContainer.scrollTop = logsContainer.scrollHeight; // Auto-scroll to bottom
        } catch (error) {
            logsContainer.textContent = 'Error fetching logs.';
            console.error(error);
        }
    };

    // Fetch and display devices
    const fetchDevices = async () => {
        try {
            const response = await axios.get('/api/lab05/devices');
            const devices = response.data;
            devicesList.innerHTML = ''; // Clear the list

            if (devices.length === 0) {
                devicesList.innerHTML = '<p>No removable drives found.</p>';
                return;
            }

            devices.forEach(device => {
                const deviceElement = document.createElement('div');
                deviceElement.className = 'device';

                const deviceInfo = document.createElement('div');
                deviceInfo.className = 'device-info';
                deviceInfo.innerHTML = `
                    <strong>${device.letter}:</strong> (${device.label || 'No label'})<br>
                    <small>Total: ${formatBytes(device.totalBytes)}, Free: ${formatBytes(device.freeBytes)}</small>
                `;

                const ejectButton = document.createElement('button');
                ejectButton.className = 'eject-btn';
                ejectButton.textContent = 'Eject';
                ejectButton.onclick = () => ejectDevice(device.letter);

                deviceElement.appendChild(deviceInfo);
                deviceElement.appendChild(ejectButton);
                devicesList.appendChild(deviceElement);
            });
        } catch (error) {
            devicesList.innerHTML = '<p>Error fetching devices.</p>';
            console.error(error);
        }
    };

    // Eject a single device
    const ejectDevice = async (driveLetter) => {
        try {
            const response = await axios.post(`/api/lab05/eject/${driveLetter}`);
            console.log(`Eject command for drive ${driveLetter} successful.`, response.data);
            return { letter: driveLetter, status: 'success', data: response.data };
        } catch (error) {
            console.error(`Error ejecting drive ${driveLetter}.`, error);
            return { letter: driveLetter, status: 'failed', error: error };
        }
    };

    // Eject all devices
    const ejectAllDevices = async () => {
        alert('Attempting to eject all connected drives...');
        try {
            const response = await axios.get('/api/lab05/devices');
            const devices = response.data;

            if (devices.length === 0) {
                alert('No removable drives to eject.');
                return;
            }

            const ejectPromises = devices.map(device => ejectDevice(device.letter));
            const results = await Promise.allSettled(ejectPromises);

            let summary = 'Ejection process finished.\\n\\nResults:\\n';
            results.forEach(result => {
                if (result.status === 'fulfilled') {
                    const { letter, status } = result.value;
                    summary += `- Drive ${letter}: ${status}\\n`;
                } else {
                    summary += `- A request failed: ${result.reason}\\n`;
                }
            });

            alert(summary);
            fetchDevices(); // Refresh device list after attempting to eject all
        } catch (error) {
            alert('Error fetching device list to eject all.');
            console.error(error);
        }
    };

    // Disable mouse
    const disableMouse = async () => {
        try {
            const response = await axios.post('/api/lab05/disable-mouse');
            alert(`Disable mouse command sent.\\nResponse: ${response.data.output}`);
        } catch (error) {
            alert('Error disabling mouse.');
            console.error(error);
        }
    };

    // Helper to format bytes
    const formatBytes = (bytes, decimals = 2) => {
        if (bytes === 0) return '0 Bytes';
        const k = 1024;
        const dm = decimals < 0 ? 0 : decimals;
        const sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB'];
        const i = Math.floor(Math.log(bytes) / Math.log(k));
        return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i];
    };

    // Add event listeners
    disableMouseButton.addEventListener('click', disableMouse);
    ejectAllButton.addEventListener('click', ejectAllDevices);

    // Initial fetch
    fetchLogs();
    fetchDevices();

    // Set intervals to refresh data
    setInterval(fetchLogs, 2000); // Refresh logs every 2 seconds
    setInterval(fetchDevices, 5000); // Refresh devices every 5 seconds
});
