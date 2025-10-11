document.getElementById('home-button').addEventListener('click', () => {
    window.location.href = '/';
});

axios.get('/api/lab02')
    .then(response => {
        const deviceList = document.getElementById('device-list');
        response.data.forEach((device, index) => {
            const item = document.createElement('div');
            item.classList.add('device-item');

            const idCell = document.createElement('div');
            idCell.classList.add('device-cell', 'id');
            idCell.textContent = index + 1;

            const vendorCell = document.createElement('div');
            vendorCell.classList.add('device-cell', 'vendor');
            vendorCell.textContent = device.vendor_name;

            const venIdCell = document.createElement('div');
            venIdCell.classList.add('device-cell', 'venid');
            venIdCell.textContent = device.ven_id;

            const devIdCell = document.createElement('div');
            devIdCell.classList.add('device-cell', 'devid');
            devIdCell.textContent = device.dev_id;

            item.appendChild(idCell);
            item.appendChild(vendorCell);
            item.appendChild(venIdCell);
            item.appendChild(devIdCell);

            deviceList.appendChild(item);
        });
    })
    .catch(error => {
        console.error('Error fetching device data:', error);
    });
