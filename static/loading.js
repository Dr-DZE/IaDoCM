const params = new URLSearchParams(window.location.search);
const redirectUrl = params.get('redirect') || '/';

setTimeout(() => {
    window.location.href = redirectUrl;
}, 1450);
