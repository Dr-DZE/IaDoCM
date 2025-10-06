document.querySelectorAll('.lab_button_1').forEach(btn => {
    btn.addEventListener('mouseenter', () => {
        document.body.style.backgroundImage = "url('/static/img/bg_1_h.png')";
    });
    btn.addEventListener('mouseleave', () => {
        document.body.style.backgroundImage = "url('/static/gif/background.gif')";
    });
});

document.querySelectorAll('.lab_button_2').forEach(btn => {
    btn.addEventListener('mouseenter', () => {
        document.body.style.backgroundImage = "url('/static/img/bg_2_h.png')";
    });
    btn.addEventListener('mouseleave', () => {
        document.body.style.backgroundImage = "url('/static/gif/background.gif')";
    });
});

document.querySelector('.lab_button_1').addEventListener('click', () => {
    window.location.href = '/loading';
});