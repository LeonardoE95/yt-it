FROM archlinux/archlinux

USER root

RUN pacman-key --init

RUN pacman -Syu --noconfirm --needed gcc nano make cmake git sfml ninja base-devel
# intel-oneapi-compiler-dpcpp-cpp-runtime-libs intel-oneapi-dpcpp-cpp
RUN rm -rf /var/cache/pacman/pkg/

ARG user=makepkg
RUN useradd --system --create-home $user \
  && echo "$user ALL=(ALL:ALL) NOPASSWD:ALL" > /etc/sudoers.d/$user
USER $user
WORKDIR /home/$user

RUN git clone https://aur.archlinux.org/yay.git \
  && cd yay \
  && makepkg -sri --needed --noconfirm \
  && cd \
  && rm -rf .cache yay

RUN yay -S --noconfirm imgui-sfml

USER root
RUN curl https://github.com/ChiefMikeK/ttf-symbola/raw/master/Symbola.ttf > /root/symbola-hint.ttf