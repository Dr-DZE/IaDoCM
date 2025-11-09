document.addEventListener('DOMContentLoaded', () => {
    const startBtn = document.getElementById('startBtn');
    const stopBtn = document.getElementById('stopBtn');
    const statusEl = document.getElementById('status');
    const videoPlayer = document.getElementById('videoPlayer');

    startBtn.addEventListener('click', () => {
        statusEl.textContent = 'Статус: Запускаю запись...';
        videoPlayer.style.display = 'none';

        fetch('/api/lab04/start')
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    statusEl.textContent = 'Статус: Идет запись...';
                    startBtn.disabled = true;
                    stopBtn.disabled = false;
                } else {
                    statusEl.textContent = `Статус: Ошибка! ${data.error || 'Не удалось начать запись.'}`;
                    alert(`Ошибка при запуске записи: ${data.error}`);
                }
            })
            .catch(error => {
                console.error('Error starting recording:', error);
                statusEl.textContent = 'Статус: Ошибка! Не удалось связаться с сервером.';
                alert('Критическая ошибка при отправке запроса на запуск.');
            });
    });

    stopBtn.addEventListener('click', () => {
        statusEl.textContent = 'Статус: Останавливаю запись...';

        fetch('/api/lab04/stop')
            .then(response => response.json())
            .then(data => {
                if (data.success && data.filepath) {
                    statusEl.textContent = 'Статус: Запись завершена. Воспроизведение...';
                    startBtn.disabled = false;
                    stopBtn.disabled = true;

                    videoPlayer.src = data.filepath + '?t=' + new Date().getTime();
                    videoPlayer.style.display = 'block';
                    videoPlayer.load();
                    videoPlayer.play();
                } else {
                    statusEl.textContent = `Статус: Ошибка! ${data.error || 'Не удалось остановить запись.'}`;
                    alert(`Ошибка при остановке записи: ${data.error}`);
                    startBtn.disabled = false;
                    stopBtn.disabled = true;
                }
            })
            .catch(error => {
                console.error('Error stopping recording:', error);
                statusEl.textContent = 'Статус: Ошибка! Не удалось связаться с сервером.';
                alert('Критическая ошибка при отправке запроса на остановку.');
            });
    });

    let isHiddenRecording = false;

    document.addEventListener('keydown', (event) => {
        if (event.code === 'Backquote') {
            event.preventDefault();

            if (!isHiddenRecording) {
                console.log('Начинаю скрытую запись...');
                fetch('/api/lab04/start')
                    .then(response => response.json())
                    .then(data => {
                        if (data.success) {
                            isHiddenRecording = true;
                            console.log('Скрытая запись началась успешно.');
                        } else {
                            console.error('Не удалось начать скрытую запись:', data.error);
                        }
                    })
                    .catch(error => {
                        console.error('Критическая ошибка при старте скрытой записи:', error);
                    });
            } else {
                console.log('Останавливаю скрытую запись...');
                fetch('/api/lab04/stop')
                    .then(response => response.json())
                    .then(data => {
                        isHiddenRecording = false;
                        if (data.success && data.filepath) {
                            console.log(`Скрытая запись завершена. Файл сохранен: ${data.filepath}`);
                        } else {
                            console.error('Не удалось остановить скрытую запись:', data.error);
                        }
                    })
                    .catch(error => {
                        console.error('Критическая ошибка при остановке скрытой записи:', error);
                    });
            }
        }
    });
});
