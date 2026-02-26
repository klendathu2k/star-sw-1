import pytest


def pytest_addoption(parser):
    parser.addoption(
        '--update-approvals',
        action='store_true',
        default=False,
        help='Regenerate golden files instead of comparing',
    )


@pytest.fixture
def update_approvals(request):
    return request.config.getoption('--update-approvals')
