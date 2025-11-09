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
    window.location.href = '/loading?redirect=/lab01';
});

document.querySelector('.lab_button_2').addEventListener('click', () => {
    window.location.href = '/loading2';
});

document.querySelector('#lab_button_3').addEventListener('mouseenter', () => {
    document.body.style.backgroundImage = "url('/static/img/bg_3.png')";
});

document.querySelector('#lab_button_3').addEventListener('mouseleave', () => {
    document.body.style.backgroundImage = "url('/static/gif/background.gif')";
});

document.querySelector('#lab_button_4').addEventListener('click', () => {
    window.location.href = '/loading?redirect=/lab04';
});
